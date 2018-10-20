#include <cache.hh>
#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <cstring>

struct Cache::Impl {
private:
	index_type memused_;
	evictor_type evictor_;
	hash_func hasher_;
	index_type maxmem_;
	std::unordered_map<std::string, void*, hash_func> data_;
	
public:
	Impl(index_type maxmem, evictor_type evictor, hash_func hasher)
	 : maxmem_(maxmem), evictor_(evictor), hasher_(hasher), memused_(0), data_(0, hasher_)
	{
	}

	~Impl() {for (auto kvpair : data_) {free(kvpair.second);}}

	void set(key_type key, val_type val, index_type size)
	{
		void *val_ptr = malloc(size);
		std::memcpy(val_ptr, val, size);
		data_[key] = val_ptr;
		memused_ += sizeof(data_[key]); //somehow increase memused
		if(memused_ > maxmem_)
		{
			del(data_.begin()->first);
		}
		//set key, value pair with key and val
	}
	
	val_type get(key_type key, index_type& val_size)
	{
		void *data_ptr = data_[key];
		if(data_ptr != 0) {return data_ptr;}
		else {return NULL;}
		//takes key and size of retrieved value
		//return a pointer to key in array
	}

	void del(key_type key)
	{
		if(data_[key] != 0)
		{
			memused_ -= sizeof(data_[key]);
			free(data_[key]);
			data_.erase(key);
		}
	}

	index_type space_used() const
	{
		return memused_;
	}
};

// Create a new cache object with a given maximum memory capacity.
Cache::Cache(index_type maxmem, evictor_type evictor, hash_func hasher)
: pImpl_(new Impl(maxmem, evictor, hasher))
{}
Cache::~Cache() = default;

// Add a <key, value> pair to the cache.
// If key already exists, it will overwrite the old value.
// Both the key and the value are to be deep-copied (not just pointer copied).
// If maxmem capacity is exceeded, sufficient values will be removed
// from the cache to accomodate the new value.
void Cache::set(key_type key, val_type val, index_type size)
{
	pImpl_ ->set(key,val,size);
}

// Retrieve a pointer to the value associated with key in the cache,
// or NULL if not found.
// Sets the actual size of the returned value (in bytes) in val_size.
Cache::val_type Cache::get(key_type key, index_type& val_size) const
{
	return pImpl_ ->get(key, val_size);
}

// Delete an object from the cache, if it's still there
void Cache::del(key_type key)
{
	pImpl_ ->del(key);
}

// Compute the total amount of memory used up by all cache values (not keys)
Cache::index_type Cache::space_used() const
{
	return pImpl_ ->space_used();
}


Cache::index_type my_hash_func(Cache::key_type key) {
	return key[0];
}

int main()
{
	Cache test_cache(100, [](){return 0;}, my_hash_func);

	char char_test[10] = "abcdefghi";
	std::string string_test = "blorp!!";
	std::string* string_test_ptr = &string_test;
	//int v[10] = {1,2,3,4,5,6,7,8,9,10};
	int int_test = 194;
	int* int_test_ptr = &int_test;

	test_cache.set("int_keya", static_cast<Cache::val_type>(int_test_ptr), sizeof(int_test));
	test_cache.set("my_keyb", static_cast<Cache::val_type>(char_test), sizeof(char_test));
	test_cache.set("str_keyc", static_cast<Cache::val_type>(string_test_ptr), sizeof(string_test));

	uint32_t asd = sizeof(int);
	int* iptr = (int*) test_cache.get("int_keya", asd);
	std::cout << *iptr << std::endl; //WORKS!!

	asd = sizeof(char_test);
	char* cptr = (char*) test_cache.get("my_keyb", asd);
	std::cout << cptr << std::endl; //WORKS!!
	
	asd = sizeof(std::string);
	std::string* sptr = (std::string*) test_cache.get("str_keyc", asd);
	std::cout << *sptr << std::endl; //WORKS!!

	test_cache.del("int_keya");
	test_cache.del("my_keyb");
	test_cache.del("str_keyc");
}
//boop