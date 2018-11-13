#include "cache.hh"
#include "queue.hh"
#include <unordered_map>
#include <cstring> //for "std::memcpy" in set

#include <iostream>
#include <stdio.h>
#include <curl/curl.h> //-lcurl
#include <fcntl.h>
#include <sys/stat.h>


int portnum = 18085;
std::string address = "localhost";

struct Cache::Impl {
private:
  index_type maxmem_;
  hash_func hasher_;
  std::unordered_map<std::string, void*, hash_func> data_;
  int PORT;

public:
  Impl(index_type maxmem, hash_func hasher) //
   : maxmem_(maxmem), hasher_(hasher), data_(0, hasher_),
    PORT(portnum) //init sock
  {
    data_.max_load_factor(0.5);
    //open socket
  }

  ~Impl()
  {
    for (auto kvpair : data_) //free all ptrs
    {
      free(data_[kvpair.first]);
      //-X DELETE localhost:18085/key
    }
    //-X POST localhost:18085/shutdown (?)
    //close the socket
  }

  //returns 0: successful set
  //returns 1: item larger than maxmem
  int set(key_type key, val_type val, index_type size)
  {
    //somehow convert val to a string
    //-X PUT localhost:18085/key/val
    //adds the k/v pair to data_
    return 0;
  }
  
  //returns ptr: successful get
  //returns NULL: no ptr associated with key
  val_type get(key_type key, index_type& val_size)
  {
    //-X GET localhost:18085/key
    //alloc a pointer to the key and put it in data_
    //if there's already one there, delete it
    //return pointer to key
    //or return NULL
  }

  //returns 0: successful delete
  //returns 1: no pointer associated with key
  int del(key_type key)
  {
    //-X DELETE localhost:18085/key
    //delete it from data_ too
    //if success return 0
    //else return 1
    return 0;
  }

  index_type space_used() const
  {
    //return -X GET localhost:18085/memsize
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

static size_t writer(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main()
{
  Cache help_me(100, NULL);
  uint32_t sz = 0;
  help_me.space_used();
  CURL *curl;
  CURLcode res;

  struct stat file_info;
  char *url;
  url = "localhost:18085/key/ah";

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  //below is post code
  /*if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, "localhost:18085/shutdown");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }*/
  //below is put code
  /*if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url);
 
    //provide the size of the upload, we specicially typecast the value
    //to curl_off_t since we must be sure to use the correct data size
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res)); 
    curl_easy_cleanup(curl);
  }*/
  //below is get code
  /*if(curl)
  {
    std::string outstring;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outstring);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      curl_easy_cleanup(curl);
    std::cout << outstring;
  }*/
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
  //below is delete code
  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl); 
  }
  curl_global_cleanup();
}