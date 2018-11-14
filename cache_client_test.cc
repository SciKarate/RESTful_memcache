#include "cache.hh"
#include <iostream>

std::string strcast(Cache::val_type vptr) //takes void ptr to str, returns str
{
	if(vptr != NULL)
	{return *((std::string*) vptr);}
	else
	{return "";}
}

//stores string pointer, then querries it
std::string basic_str_set_get(Cache::val_type ptr, uint32_t sz)
{
	uint32_t blnk = 0;
	Cache test_cache(sz); //create a cache
	test_cache.set("bl", ptr, blnk);
	std::string outstr = strcast(test_cache.get("bl",blnk));
	std::cout << "Stored:\t" << strcast(ptr) << "\tRetrieved:\t" << outstr << std::endl;
	return outstr;
}

void super_basic_test()
{
	using namespace std;
	Cache help_me(100);
	uint32_t sz = 0;
	string real = "hello";
	Cache::val_type fake = &real;
	cout << "This is the value we're storing: " << real << endl;
	cout << "And this is its address: " << fake << endl;
	cout << "Now, we're setting it. " << help_me.set("bl", fake, sz) << endl;
	Cache::val_type save_get = help_me.get("bl",sz);
	cout << "Here's its new address from the server. " << save_get << endl;
	cout << "The data at the address is: " << *(std::string*) save_get << endl;
	cout << "Space used is currently: " << help_me.space_used() << endl;
	cout << "Now we delete: " << help_me.del("bl") << endl;
	cout << "Space used is now: " << help_me.space_used() << endl;
	cout << endl;
}

void just_make_it()
{
	Cache hello(5);
	Cache extra(7);
	std::string real = "howdy";
	Cache::val_type fake = &real;
	hello.set("kek", fake, 6);
	std::cout << "First one!" << std::endl;
	extra.set("eke", fake, 6);
	std::cout << "Second one!" << std::endl;
}

int main()
{
	//test code
	std::string b = "hellothere";
	Cache::val_type bp = &b;
	just_make_it();
	//super_basic_test();
	//basic_str_set_get(bp, 100);
}