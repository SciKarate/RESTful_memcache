#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <cache.hh>
#include <iostream>
#include <functional>
//build with "g++ cache.cc cache_tester.cc"

int intcast(Cache::val_type vptr) //takes void ptr to int, returns int
{
	if(vptr != 0)
		{return *((int*) vptr);}
	else
		{return 0;}
}

std::string strcast(Cache::val_type vptr) //takes void ptr to str, returns str
{
	if(vptr != 0)
		{return *((std::string*) vptr);}
	else
		{return "0";}
}

std::string chararrcast(Cache::val_type vptr) //takes void ptr to char array, returns str
{
	if(vptr != 0)
	{
		return (std::string)((char*) vptr);
	}
	else
		{return "0";}
}

Cache::index_type my_hash_func(Cache::key_type key)
{
	std::hash<std::string> hashy;
	return hashy(key);
}

void cache_test()
{
	Cache test_cache(38, [](){return 0;}, my_hash_func); //create a cache

	//create some test variables to store
	char char_test[10] = "abcdefghi";
	std::string string_test = "blorp!!";
	std::string* string_test_ptr = &string_test;
	int int_test = 194;
	int* int_test_ptr = &int_test;
	int int_test2 = 172;
	int* int_test_ptr2 = &int_test2;

	//store all those test variables
	test_cache.set("str_keyc", static_cast<Cache::val_type>(string_test_ptr), sizeof(string_test));
	std::cout << "Storing string..." << "\t\t" << test_cache.space_used() << std::endl;

	test_cache.set("int_keya", static_cast<Cache::val_type>(int_test_ptr), sizeof(int_test));
	std::cout << "Storing int..." << "\t\t\t" << test_cache.space_used() << std::endl;

	test_cache.set("my_keyb", static_cast<Cache::val_type>(char_test), sizeof(char_test));
	std::cout << "Storing char array..." << "\t\t" << test_cache.space_used() << std::endl;

	/*int intarr_test[10] = {1,2,3,4,5,6,7,8,9,10};
	test_cache.set("intarr_keyd", static_cast<Cache::val_type>(intarr_test), sizeof(intarr_test));
	std::cout << "Storing int array..." << "\t\t" << test_cache.space_used() << std::endl;*/

	using namespace std;
	uint32_t sz; //used to store size for get calls

	//fun with ints! we...
	//1) get our stored value
	//2) try to pull out a value that is not stored
	//3) modify a stored value
	//4) get our new, modified value out.

	sz = sizeof(int);
	cout << "Stored 194:\t\t" << intcast(test_cache.get("int_keya", sz)) << "\t\t" << test_cache.space_used() << endl;
	cout << "Unstored int:\t\t" << intcast(test_cache.get("int_key", sz)) << "\t\t" << test_cache.space_used() << endl;
	test_cache.set("int_keya", static_cast<Cache::val_type>(int_test_ptr2), sizeof(int_test2));
	cout << "Modified 172:\t\t" << intcast(test_cache.get("int_keya", sz)) << "\t\t" << test_cache.space_used() << endl;

	sz = sizeof(char_test);
	cout << "Stored abcdefghi:\t" << chararrcast(test_cache.get("my_keyb", sz)) << "\t" << test_cache.space_used() << endl;
	
	sz = sizeof(string);
	cout << "Evicted blorp!!:\t" << strcast(test_cache.get("str_keyc", sz)) << "\t\t" << test_cache.space_used() << endl;
	
	test_cache.del("int_keya");
	test_cache.del("my_keyb");
	test_cache.del("str_keyc");

	sz = sizeof(int);
	cout << "Deleted int:\t\t" << intcast(test_cache.get("int_keya", sz)) << "\t\t" << test_cache.space_used() << endl;

	sz = sizeof(char_test);
	cout << "Deleted chararr:\t" << chararrcast(test_cache.get("my_keyb", sz)) << "\t\t" << test_cache.space_used() << endl;
	
	sz = sizeof(string);
	cout << "Deleted, evicted str:\t" << strcast(test_cache.get("str_keyc", sz)) << "\t\t" << test_cache.space_used() << endl;
	test_cache.set("str_keyc", static_cast<Cache::val_type>(string_test_ptr), sizeof(string_test));
	cout << "Stored blorp!!:\t\t" << strcast(test_cache.get("str_keyc", sz)) << "\t\t" << test_cache.space_used() << endl;

	cout << "There's still a string stored in the map, but I bet you there's no memory leaks!" << endl;
}

void cache_test_cacheflush()
{
    using namespace std;
    Cache test_cache(41, [](){return 0;}, my_hash_func); //create a cache
    bool a = false;
    bool* aptr = &a;
    test_cache.set("bool_key", static_cast<Cache::val_type>(aptr),sizeof(a));
    cout << "Storing boolean..." << "\t\t" << test_cache.space_used() <<endl;

    double b = 5287.47463;
    double* bptr = &b;
    test_cache.set("double_key", static_cast<Cache::val_type>(bptr),sizeof(b));
    cout << "Storing double..." << "\t\t" << test_cache.space_used() <<endl;

    int intarr_test[10] = {1,2,3,4,5,6,7,8,9,10};
    test_cache.set("intarr_keyd", static_cast<Cache::val_type>(intarr_test), sizeof(intarr_test));
    cout << "Storing int array..." << "\t\t" << test_cache.space_used() <<endl;


}

void cache_test_samekey()
{
    uint32_t sz;
    using namespace std;

    int cache_size = 82;
    Cache test_cache(cache_size, [](){return 0;}, my_hash_func); //create a cache
    cout<<"Size of cache:\t\t"<<cache_size<<endl;
    int a = 12;
    int* aptr = &a;
    test_cache.set("int_key", static_cast<Cache::val_type>(aptr),sizeof(a));
    cout << "Storing int..." << "\t\t" << test_cache.space_used() <<endl;

    cout << "Stored a!!:\t\t" << intcast(test_cache.get("int_key", sz)) << "\t\t" << test_cache.space_used() << endl;

    string b = "apple";
    string* bptr = &b;

    cout<< "this is size of b:\t\t"<< sizeof(b)<<endl;

    test_cache.set("int_key", static_cast<Cache::val_type>(bptr),sizeof(b));
    cout << "Storing string..." << "\t\t" << test_cache.space_used() <<endl;

    cout << "Stored b!!:\t\t" << strcast(test_cache.get("int_key", sz)) << "\t\t" << test_cache.space_used() << endl;
    //successfully replaces int with a string if same key is used

    int c = 13;
    int* cptr = &c;
    test_cache.set("int_key", static_cast<Cache::val_type>(cptr),sizeof(c));
    cout << "Storing int..." << "\t\t" << test_cache.space_used() <<endl;

    cout << "Stored c!!:\t\t" << intcast(test_cache.get("int_key", sz)) << "\t\t" << test_cache.space_used() << endl;
}


//tests attempts to query empty cache
void cache_test_emptyquery()
{
	uint32_t sz;
    using namespace std;
    int cache_size = 32;
    Cache test_cache(cache_size, [](){return 0;}, my_hash_func); //create a cache

    //cout<<"Size of cahce:\t\t"<<cache_size<<endl;

    //quering someting that doesn't exist
    sz = sizeof(int);
	cout << "Nonexistent int:\t\t" << intcast(test_cache.get("int_keya", sz)) << "\t\t" << test_cache.space_used() << endl;
}

//test attempts to query a nonexistent key in non empty cache
void cache_test_wrongquery()
{
	uint32_t sz;
    using namespace std;
    int cache_size = 32;
    Cache test_cache(cache_size, [](){return 0;}, my_hash_func); //create a cache

    int a = 12;
    int* aptr = &a;
    test_cache.set("int_keyA", static_cast<Cache::val_type>(aptr),sizeof(a));
    cout << "Storing int..." << "\t\t" << test_cache.space_used() <<endl;

    cout << "Stored a?!:\t\t" << intcast(test_cache.get("int_keyB", sz)) << "\t\t" << test_cache.space_used() << endl;

}
//test querying a key after it's been deleted
void cache_test_delquery()
{
	uint32_t sz;
    using namespace std;
    int cache_size = 32;
    Cache test_cache(cache_size, [](){return 0;}, my_hash_func);

    int a = 12;
    int* aptr = &a;
    test_cache.set("int_keya", static_cast<Cache::val_type>(aptr),sizeof(a));
    cout << "Storing int..." << "\t\t" << test_cache.space_used() <<endl;

    test_cache.del("int_keya");

    cout << "deleted int:\t\t" << intcast(test_cache.get("int_keya", sz)) << "\t\t" << test_cache.space_used() << endl;

}

//test querying a key after its been evicted
void cache_test_evictedquery()
{
	uint32_t sz;
	//~copypasta cacheflush
	using namespace std;
    Cache test_cache(41, [](){return 0;}, my_hash_func); //create a cache
    bool a = false;
    bool* aptr = &a;
    test_cache.set("bool_key", static_cast<Cache::val_type>(aptr),sizeof(a));
    cout << "Storing boolean..." << "\t\t" << test_cache.space_used() <<endl;

    int intarr_test[10] = {1,2,3,4,5,6,7,8,9,10};
    test_cache.set("intarr_keyd", static_cast<Cache::val_type>(intarr_test), sizeof(intarr_test));
    cout << "Storing int array..." << "\t\t" << test_cache.space_used() <<endl;

    cout << "query evicted bool!:\t\t" << intcast(test_cache.get("bool", sz)) << "\t\t" << test_cache.space_used() << endl;


}
//sample
uint32_t basic_delete(Cache::val_type ptr, uint32_t sz)
{
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("key", ptr, sz);
    test_cache.del("key");
    uint32_t outint = intcast(test_cache.get("key",sz));
    std::cout << "Stored:\t" << "Pointer to delete" << "\tRetrieved:\t" << outint << std::endl;
    return outint;
}

//deletes key from brand new cache
std::string new_cache_delete()
{
	Cache test_cache(4, [](){return 0;}, my_hash_func); //create a cache
	test_cache.del("key");
	std::string yay = "cache is not broken!";
	return yay;
}


TEST_CASE()
{
	/*int a = 12;
	std::string b = "hello";
	int f[10] = {1,2,3,4,5,6,7,8,9,10};
	Cache::val_type ap = &a;
	Cache::val_type bp = &b;
	Cache::val_type fp = &f;
	int as = sizeof(a); int bs = sizeof(b); int fs = sizeof(f);
	*/
	REQUIRE(new_cache_delete() =="cache is not broken!");

}
//boop