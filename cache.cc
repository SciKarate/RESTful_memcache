#include <cache.hh>
#include <unordered_map>
#include <cstring> //for "std::memcpy" in set
#include <iostream>

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

	~Impl() {for (auto kvpair : data_) {del(kvpair.first);}}

	void set(key_type key, val_type val, index_type size)
	{
		if(data_[key] != 0) {free(data_[key]);}
		void *val_ptr = malloc(size);
		std::memcpy(val_ptr, val, size);
		data_[key] = val_ptr;
		memused_ += sizeof(data_[key]); //somehow increase memused
		if(memused_ > maxmem_)
		{
			std::cout << "evicting key...\t\t" << (data_.begin()->first) << std::endl;
			del(data_.begin()->first);
		}
		//set key, value pair with key and val
	}
	
	val_type get(key_type key, index_type& val_size)
	{
		if(data_[key] != 0) {return data_[key];}
		else {data_.erase(key); return NULL;}
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