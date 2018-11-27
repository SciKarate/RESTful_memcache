//g++ testing_client.cc cache_client_test.cc -o cl.out -lboost_system -pthread -lcurl -ljsoncpp
//needs: boost_system, libcurl, jsoncpp
#include "cache.hh"
#include "h_depen/queue.hh"
#include <unordered_map>
#include <cstring> //for "std::memcpy" in set
#include <iostream>
#include <curl/curl.h> //-lcurl
#include <sys/stat.h>
#include <jsoncpp/json/json.h>

std::string address = "localhost";
std::string portnum = "18085";

static size_t writer(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

struct Cache::Impl {
private:
	CURL *curl;
	CURLcode res;
	index_type maxmem_;
	hash_func hasher_;
	std::unordered_map<std::string, std::string*, hash_func> data_;
	std::string surl;

public:
	Impl(index_type maxmem, hash_func hasher) //
	 : maxmem_(maxmem), hasher_(hasher), data_(0, hasher_)
	{
		surl += address;
		surl += ":";
		surl += portnum;
		data_.max_load_factor(0.5);
		curl_global_init(CURL_GLOBAL_ALL);
	}

	~Impl()
	{
		for (auto kvpair : data_) //free all ptrs
		{
			//del(kvpair.first);
			delete(data_[kvpair.first]);
			data_.erase(kvpair.first);
		}
		curl = curl_easy_init();
		curl_global_cleanup();
	}

	//returns 0: successful set
	//returns 1: item larger than maxmem
	int set(key_type key, val_type val, index_type size)
	{
		curl = curl_easy_init();
		
		//somehow convert val to a string
		std::string valstring = *((std::string*) val);

		std::string new_url = surl + "/key/" + key + "/" + valstring;
		const char* url = new_url.c_str();
		//std::cout << url << std::endl;
		if(curl) //-X PUT localhost:18085/key/val
		{
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
			curl_easy_setopt(curl, CURLOPT_URL, url);
 
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
				{
					fprintf(stderr, "set(k, v, sz) failed:\t %s\n", curl_easy_strerror(res));
					return 1;
				}
			curl_easy_cleanup(curl);
		}
		return 0;
	}
	
	//returns ptr: successful get
	//returns NULL: no ptr associated with key
	val_type get(key_type key, index_type& val_size)
	{
		curl = curl_easy_init();
		std::string outstring;
		std::string new_url = surl + "/key/" + key;
		const char* url = new_url.c_str();

		if(curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outstring);
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
				{
					fprintf(stderr, "get(k, v_s) failed:\t %s\n", curl_easy_strerror(res));
					return NULL;
				}
			curl_easy_cleanup(curl);
		}
		//std::cout << outstring << std::endl;

		Json::Value root;	
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( outstring.c_str(), root );	 //parse process
		if ( !parsingSuccessful )
			{return NULL;}
		std::string valler = root.get("value", "NOT FOUND" ).asString();

		//somehow allocate memory for valler and turn it into a void*
		std::string *vptr = new std::string(valler);
		const auto& str = *static_cast<const std::string*>(vptr);
		val_size = str.size() + 1;

		//remember the pointer so we can clean up at the end
		data_[key] = vptr;
		return (const void*)vptr;
	}

	//returns 0: successful delete
	//returns 1: no pointer associated with key
	int del(key_type key)
	{
		curl = curl_easy_init();
		int retcode = 0;
		std::string new_url = surl + "/key/" + key;
		const char* url = new_url.c_str();
		if(curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
				{retcode = 1;}
			curl_easy_cleanup(curl); 
		}
		return retcode;
	}

	index_type space_used()
	{
		int retval = 0;
		curl = curl_easy_init();
		std::string outstring;
		std::string new_url = surl + "/memsize";
		const char* url = new_url.c_str();
		if(curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outstring);
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
				{fprintf(stderr, "space_used() failed:\t %s\n", curl_easy_strerror(res));}
			curl_easy_cleanup(curl);
		}
		Json::Value root;	
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( outstring.c_str(), root );	 //parse process
		if ( !parsingSuccessful )
		{
			return 0;
		}
		std::string valler = root.get("memused", "NOT FOUND" ).asString();
		retval = stoi(valler);
		return retval;
	}
};

// Create a new cache object with a given maximum memory capacity.
Cache::Cache(index_type maxmem, hash_func hasher)
: pImpl_(new Impl(maxmem, hasher))
{}
Cache::~Cache() = default;

// Add a <key, value> pair to the cache.
// If key already exists, it will overwrite the old value.
// Both the key and the value are to be deep-copied (not just pointer copied).
// If maxmem capacity is exceeded, sufficient values will be removed
// from the cache to accomodate the new value.
int Cache::set(key_type key, val_type val, index_type size)
{
	return pImpl_ ->set(key,val,size);
}

// Retrieve a pointer to the value associated with key in the cache,
// or NULL if not found.
// Sets the actual size of the returned value (in bytes) in val_size.
Cache::val_type Cache::get(key_type key, index_type& val_size) const
{
	return pImpl_ ->get(key, val_size);
}

// Delete an object from the cache, if it's still there
int Cache::del(key_type key)
{
	return pImpl_ ->del(key);
}

// Compute the total amount of memory used up by all cache values (not keys)
Cache::index_type Cache::space_used() const
{
	return pImpl_ ->space_used();
}