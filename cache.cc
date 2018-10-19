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

	~Impl() = default;

	void set(key_type key, val_type val, index_type size)
	{
		if(memused_ >= maxmem_)
		{
			printf("this is where I would evict things\n");
			std::cout << memused_ << std::endl;
		}
		/*char* val_ptr = (char*)val; //cast val as real ptr
		std::cout<<val_ptr<<std::endl;
		char outt[size];
		for(int i = 0; i < size; i++) //char outt = *val_ptr;
		{
			outt[i] = *(val_ptr + i);
		}*/
		//void* val_ptr;
		//std::memcpy(val_ptr, val, size);
		//std::cout<< val_ptr <<std::endl;
		//std::cout<<outt<<std::endl; //check successful copy
		//void* outt_ptr = static_cast<void*>(outt); //save as void*
		//char* extractedboyo = (char*)val_ptr;
		//std::cout<< extractedboyo << std::endl;
		data_[key] = val_ptr;
		std::cout << key << std::endl;
		std::cout << val << std::endl;
		std::cout << data_[key] << std::endl;
		printf("god is ded\n\n");
		memused_ += 1; //somehow increase memused
		//set key, value pair with key and val
	}
	
	val_type get(key_type key, index_type& val_size) const
	{
		//takes key and size of retrieved value
		//return a pointer to key in array
	}

	void del(key_type key)
	{
		if(data_[key] != 0) {memused_ -= 1; data_[key] = 0;}
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

	char v[10] = "abcdefghi";
	std::string s = "blorp!!";
	std::string* sss = &s;
	//int v[10] = {1,2,3,4,5,6,7,8,9,10};
	int x = 10; int* z = &x;
	test_cache.set("int_keya", static_cast<Cache::val_type>(z), sizeof(x));
	test_cache.set("my_keyb", static_cast<Cache::val_type>(v), sizeof(v));
	test_cache.set("str_keyc", static_cast<Cache::val_type>(sss), sizeof(s));
}

//boop
/*
unordered_map<std::string, void*, hash_func> my_table(0, hasher_);
hash_func hasher;
hash_vale = hasher();
std::hash<std::string> h;
h();
struct Cache::Impl {
	unordered_map<std::string, void*, hash_func> data_;
	...
	Impl(maxmem, hasher, evictor)
	: data_(0, hasher)
}
*/