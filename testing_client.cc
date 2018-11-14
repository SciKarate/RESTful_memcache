//g++ testing_client.cc cache_client_test.cc -o cl.out -lboost_system -pthread -lcurl -ljsoncpp
//needs: boost_system, libcurl, jsoncpp
#include "cache.hh"
#include "queue.hh"
#include <unordered_map>
#include <cstring> //for "std::memcpy" in set

#include <iostream>
#include <stdio.h>
#include <curl/curl.h> //-lcurl
#include <fcntl.h>
#include <sys/stat.h>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>

std::string address = "localhost";
std::string portnum = "18085";

//used for libcurl requests
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
			del(kvpair.first);
			curl_global_cleanup();
		}
	}

	//returns 0: successful set
	//returns 1: item larger than maxmem
	int set(key_type key, val_type val, index_type size)
	{
		struct stat file_info;
		curl = curl_easy_init();
		
		//somehow convert val to a string
		std::string valstring = *((std::string*) val);

		std::string new_url = surl + "/key/" + key + "/" + valstring;
		const char* url = new_url.c_str();
		
		if(curl) //-X PUT localhost:18085/key/val
		{
			curl_easy_setopt(curl, CURLOPT_PUT, 1L);
			curl_easy_setopt(curl, CURLOPT_URL, url);
 
			//provide the size of the upload, we specicially typecast the value
			//to curl_off_t since we must be sure to use the correct data size
			curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
			std::cout << "Oh my" << std::endl;
			res = curl_easy_perform(curl);
			std::cout << "goodness." << std::endl;
			if(res != CURLE_OK)
				{fprintf(stderr, "set(k, v, sz) failed:\t %s\n", curl_easy_strerror(res));}
			curl_easy_cleanup(curl);
		}
		return 0;
	}
	
	//returns ptr: successful get
	//returns NULL: no ptr associated with key
	val_type get(key_type key, index_type& val_size)
	{
		void* val = NULL;
		
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
				{fprintf(stderr, "get(k, v_s) failed:\t %s\n", curl_easy_strerror(res));}
			curl_easy_cleanup(curl);
		}

		Json::Value root;	
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( outstring.c_str(), root );	 //parse process
		if ( !parsingSuccessful )
			{return NULL;}
		std::string valler = root.get("key", "NOT FOUND" ).asString();

		//somehow allocate memory for valler and turn it into a void*
		std::string *vptr = new std::string(valler);
		//set val_size = size of valler
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
		delete(data_[key]);
		data_.erase(key);
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


/*int main()
{
	//below is header code
	if(curl)
	{
		std::string outstring;
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outstring);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
		std::cout << outstring;
	}
	//below is shutdown code
		if(curl) //-X POST localhost:18085/shutdown
		{
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
			res = curl_easy_perform(curl);
			if(res != CURLE_OK)
				{fprintf(stderr, "shutdown failed:\t %s\n", curl_easy_strerror(res));}
			curl_easy_cleanup(curl);
		}
}*/