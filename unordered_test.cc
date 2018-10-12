#include <cache.hh>
#include <iostream>
#include <unordered_map>
#include <string>

using key_type = const std::string;
using val_type = const void*;
using hash_func = std::function<uint32_t(key_type)>;
std::unordered_map<std::string, void*> data_;

void assign(key_type key, val_type val)
{
	void* val_p = &val;
	data_[key] = val_p;
}

int main()
{	
	key_type k = "help"; 
	val_type v = "apple";
	assign(k, v);
	std::cout << data_ [k];
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

Cache::index_type my_hash_func(Cache::key_type key) {
	return key[0];
}


struct MyHasher {
	int data_;
	MyHasher() : data_(0) {}

	Cache::index_type operator()(Cache::key_type key) {
		return key[0];
	}
};

MyHasher hs;

hs("yohoo!");


Cache test_cache(100, [](){return 0;}, my_hash_func);
Cache test_cache(100, [](){return 0;}, hs);

char data[10] = "abcdefghi";

test_cache.set("my_key", static_cast<Cache::val_type>(data), 10);

int size;
p = test_cache.get("my_key", size)

