#include <cache.hh>
#include <iostream>
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
	return key[0];
}

int main()
{	//set mem to 32 for UNIX, 16 for Win for tests to evict as expected
	Cache test_cache(32, [](){return 0;}, my_hash_func); //create a cache

	//create some test variables to store
	char char_test[10] = "abcdefghi";
	std::string string_test = "blorp!!";
	std::string* string_test_ptr = &string_test;
	int intarr_test[10] = {1,2,3,4,5,6,7,8,9,10};
	int int_test = 194;
	int* int_test_ptr = &int_test;
	int int_test2 = 172;
	int* int_test_ptr2 = &int_test2;

	//store all those test variables
	test_cache.set("int_keya", static_cast<Cache::val_type>(int_test_ptr), sizeof(int_test));
	test_cache.set("my_keyb", static_cast<Cache::val_type>(char_test), sizeof(char_test));
	test_cache.set("str_keyc", static_cast<Cache::val_type>(string_test_ptr), sizeof(string_test));
	test_cache.set("intarr_keyd", static_cast<Cache::val_type>(intarr_test), sizeof(intarr_test));

	using namespace std;
	uint32_t sz; //used to store size for get calls

	//fun with ints! we...
	//1) get our stored value
	//2) try to pull out a value that is not stored
	//3) modify a stored value
	//4) get our new, modified value out.
	sz = sizeof(int);
	cout << "Stored 194:\t\t" << intcast(test_cache.get("int_keya", sz)) << endl;
	cout << "Unstored int:\t\t" << intcast(test_cache.get("int_key", sz)) << endl;
	test_cache.set("int_keya", static_cast<Cache::val_type>(int_test_ptr2), sizeof(int_test2));
	cout << "Modified 172:\t\t" << intcast(test_cache.get("int_keya", sz)) << endl;

	sz = sizeof(char_test);
	cout << "Stored abcdefghi:\t" << chararrcast(test_cache.get("my_keyb", sz)) << endl;
	
	sz = sizeof(string);
	cout << "Evicted blorp!!:\t" << strcast(test_cache.get("str_keyc", sz)) << endl;
	
	test_cache.del("int_keya");
	test_cache.del("my_keyb");
	test_cache.del("str_keyc");

	sz = sizeof(int);
	cout << "Deleted int:\t\t" << intcast(test_cache.get("int_keya", sz)) << endl;

	sz = sizeof(char_test);
	cout << "Deleted chararr:\t" << chararrcast(test_cache.get("my_keyb", sz)) << endl;
	
	sz = sizeof(string);
	cout << "Deleted, evicted str:\t" << strcast(test_cache.get("str_keyc", sz)) << endl;
	test_cache.set("str_keyc", static_cast<Cache::val_type>(string_test_ptr), sizeof(string_test));
	cout << "Stored blorp!!:\t\t" << strcast(test_cache.get("str_keyc", sz)) << endl;

	cout << "There's still a string stored in the map, but I bet you there's no memory leaks!" << endl;
}
//boop