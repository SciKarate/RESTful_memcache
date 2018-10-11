#include <cache.hh>

struct Cache::Impl {
	index_type memused_;
	evictor_type evictor_;
	hash_func hasher_;
	index_type maxmem_;
public:
	Impl(index_type maxmem, evictor_type evictor, hash_func hasher)
	 : maxmem_(maxmem), evictor_(evictor), hasher_(hasher), memused_(0)
	{
	}

	~Impl() = default;

	void set(key_type key, val_type val, index_type size)
	{
	}
	
	val_type get(key_type key, index_type& val_size) const
	{
	}

	void del(key_type key)
	{
	}

	index_type space_used() const
	{
		return memused_;
	}
};

// Create a new cache object with a given maximum memory capacity.
Cache::Cache(index_type maxmem,
	evictor_type evictor,
	hash_func hasher)
 : pImpl_()
{
}

Cache::~Cache()
{
}

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

uint32_t hasho(const std::string x)
{
	return 10;
}

uint32_t evicto(void)
{
	return 10;
}

int main()
{
	Cache test_cache(10);
	test_cache.space_used();
}

//boop