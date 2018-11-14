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
  index_type maxmem_;
  hash_func hasher_;
  std::unordered_map<std::string, void*, hash_func> data_;
  std::string surl;

public:
  Impl(index_type maxmem, hash_func hasher) //
   : maxmem_(maxmem), hasher_(hasher), data_(0, hasher_)
  {
    surl += address;
    surl += ":";
    surl += portnum;
    data_.max_load_factor(0.5);
  }

  ~Impl()
  {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    std::string new_url = surl + "/shutdown";
    const char* url = new_url.c_str();
    for (auto kvpair : data_) //free all ptrs
    {
      free(data_[kvpair.first]);
    }
    if(curl) //-X POST localhost:18085/shutdown
    {
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
        {fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));}
      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
  }

  //returns 0: successful set
  //returns 1: item larger than maxmem
  int set(key_type key, val_type val, index_type size)
  {
    struct stat file_info;
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    std::string new_url = surl + "/key/" + key + "/hi";
    const char* url = new_url.c_str();
    //somehow convert val to a string
    if(curl) //-X PUT localhost:18085/key/val
    {
      curl_easy_setopt(curl, CURLOPT_PUT, 1L);
      curl_easy_setopt(curl, CURLOPT_URL, url);
 
      //provide the size of the upload, we specicially typecast the value
      //to curl_off_t since we must be sure to use the correct data size
      curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);

      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
        {fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));}
      curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
  }
  
  //returns ptr: successful get
  //returns NULL: no ptr associated with key
  val_type get(key_type key, index_type& val_size)
  {
    void* val = NULL;
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
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
        {fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));}
      curl_easy_cleanup(curl);
      std::cout << outstring;
    }
    curl_global_cleanup();

    Json::Value root;   
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( outstring.c_str(), root );     //parse process
    if ( !parsingSuccessful )
    {
        return NULL;
    }
    std::string valler = root.get("key", "A Default Value if not exists" ).asString();
    std::cout << valler << std::endl;

    data_[key] = val;
    return val;
  }

  //returns 0: successful delete
  //returns 1: no pointer associated with key
  int del(key_type key)
  {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
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
    curl_global_cleanup();
    return retcode;
  }

  index_type space_used() const
  {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
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
        {fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));}
      curl_easy_cleanup(curl);
      std::cout << outstring;
    }
    curl_global_cleanup();
    Json::Value root;   
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( outstring.c_str(), root );     //parse process
    if ( !parsingSuccessful )
    {
        return 0;
    }
    std::string valler = root.get("memused", "A Default Value if not exists" ).asString();
    std::cout << valler << std::endl;
    return 5;
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

int main()
{
  Cache help_me(100);
  uint32_t sz = 0;
  Cache::val_type fake;
  help_me.set("bl", fake, sz);
  help_me.get("bl", sz);
  help_me.space_used();
  help_me.del("bl");
  help_me.space_used();

  //below is header code
  /*if(curl)
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
  }*/
}