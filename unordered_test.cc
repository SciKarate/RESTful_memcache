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
	using namespace std;
	key_type k = "help"; 
	val_type v = "apple";
	assign(k, v);
	cout << data_[k] << endl;
	cout << endl;
	
	int myvar = 25;
	void* val = &myvar;
	int* val_ptr = (int*)val;
	int apple = *val_ptr;
	cout << myvar << endl;
	cout << val << endl;
	cout << apple << endl;
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


/*
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
*/

		/*
		char* val_ptr = (char*)val; //cast val as real ptr
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