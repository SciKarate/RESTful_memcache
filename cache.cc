#include <cache.hh>
#include <iostream>
#include <unordered_map>
#include <string>

struct Cache::Impl {
private:
	index_type memused_;
	evictor_type evictor_;
	hash_func hasher_;
	index_type maxmem_;
	std::unordered_map<std::string, void*, hash_func> data_;
	
public:
	/*Impl(index_type maxmem, evictor_type evictor, hash_func hasher)
	 : maxmem_(maxmem), evictor_(evictor), hasher_(hasher), memused_(0)
	{
	}*/

	//~Impl() = default;
	void do_internal_work(index_type maxmem, evictor_type evictor, hash_func hasher)
	{
		maxmem_ = maxmem;
		evictor_ = evictor;
		hasher_ = hasher;
		memused_ = 0;
	}

	void set(key_type key, val_type val, index_type size)
	{
		hash_func hasher = std::hash<std::string>();
		size_t hashed = hasher(key);
		//set key, value pair with key and val
	}
	
	val_type get(key_type key, index_type& val_size) const
	{
		hash_func hasher = std::hash<std::string>();
		size_t hashed = hasher(key);
		//return a pointer to key in array
	}

	void del(key_type key)
	{
		printf("let's test delete\n");
		hash_func hasher = std::hash<std::string>();
		size_t hashed = hasher(key);
		//if there's anything at hashed in array, delete it n
	}

	index_type space_used() const
	{
		return memused_;
	}
};

// Create a new cache object with a given maximum memory capacity.
Cache::Cache(index_type maxmem, evictor_type evictor, hash_func hasher)
: pImpl_(new Impl) {pImpl_ ->do_internal_work(maxmem, evictor, hasher);}

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
Cache::val_type
Cache::get(key_type key, index_type& val_size) const
{
	return pImpl_ ->get(key, val_size);
}

// Delete an object from the cache, if it's still there
void Cache::del(key_type key)
{
	pImpl_ ->del(key);
}

// Compute the total amount of memory used up by all cache values (not keys)
Cache::index_type
Cache::space_used() const
{
	return pImpl_ ->space_used();
}

int main()
{
	Cache test_cache(10);
	std::cout << test_cache.space_used() << std::endl;
	test_cache.del("help");
	Cache::val_type x = "8";
	Cache::index_type y = 10;
	Cache::key_type z = "zoop";
	test_cache.set(z,x,y);
}

//boop
/*
unordered_map<std::string, void*, hash_func> my_table(0, hasher_;
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