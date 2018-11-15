//g++ testing_client_shutdown.cc cache_client_test_shutdown.cc -o cls.out -lboost_system -pthread -lcurl -ljsoncpp
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "cache.hh"
#include <iostream>
#include <functional>

Cache test_cache(100); //create a cache

std::string strcast(Cache::val_type vptr) //takes void ptr to str, returns str
{
	if(vptr != NULL)
		{return *((std::string*) vptr);}
	else
		{return "";}
}

/*probably all wrong
std::string new_cache_delete(std::string newkey)
{
    test_cache.del(newkey);
    return "cache is not broken!\n";
}

std::string new_cache_get(std::string newkey, uint32_t blnk)
{
    test_cache.get(newkey,blnk);
    return "cache is not broken!\n";
}

std::string basic_set_get(std::string newkey, Cache::val_type ap, int as)
{
    test_cache.set("newk", ap, as);
}
*/

std::string basic_str_set_get(Cache::val_type ptr, uint32_t sz)
{
    uint32_t blnk = 0;
    test_cache.set("key", ptr, sz);
    std::string outstr = strcast(test_cache.get("key",blnk));
    std::cout << "Stored:\t" << strcast(ptr) << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

TEST_CASE("test shutdown version functionality")
{
    std::string a = "hello";
    Cache::val_type ap = &a;
    int as = sizeof(a);
    uint32_t blnk = 0;


    SECTION("basic set get")//testing setting a k,v pair and retreiving v
    {
        REQUIRE(basic_str_set_get(ap,as) == a);
        std::cout << "\n";
    }
    SECTION("new cache delete")//testing delete in empty cache
    {
        REQUIRE(test_cache.del("newk") == 1);
        std::cout << "\n";
    }
    SECTION("new cache get")//testing get in empty cache
    {
        REQUIRE(test_cache.get("newk",blnk) == "NULL");
        std::cout << "\n";
    }
    SECTION("basic delete")//testing deleting item from cache
    {
        test_cache.del("newk");
        std::string outstr = strcast(test_cache.get("newk",blnk));
        REQUIRE(outstr == "NULL");
        std::cout << "\n";
    }
    SECTION("basic memused")//testing memused of cache
    {
        test_cache.set("newk", ap, as);
        REQUIRE(test_cache.space_used()==5);
        std::cout << "\n";
    }
    SECTION("basic deep copy")
    {
        test_cache.set("newk", ap, as);
        REQUIRE(test_cache.get("newk",blnk) != ap);
        std::cout << "\n";

    }



}
/*
int main()
{
	
	std::string a = "hello";
	Cache::val_type ap = &a;
	int as = sizeof(a);
	uint32_t blnk = 0;

	//new cache_delete
	test_cache.del("newk");
    std::cout << "cache is not broken!\n";

    //new cache get
    test_cache.get("newk",blnk);
    std::cout << "cache is not broken!\n";

	//basic set get
    test_cache.set("newk", ap, as);
    std::string outstr = strcast(test_cache.get("newk",blnk));
    std::cout << "Stored:\t" << strcast(ap) << "\tRetrieved:\t" << outstr << std::endl;

    //basic delete
    test_cache.set("newk", ap, as);
    test_cache.del("newk");
    outstr = strcast(test_cache.get("newk",blnk));
    std::cout << "Stored:\t" << "Pointer to delete" << "\tRetrieved:\t" << outstr << std::endl;

    //basic memused
    test_cache.set("newk", ap, as);
    std::cout << "Verifying space_used...\n";
    std::cout << test_cache.space_used() << std::endl;

    //verify deep copy
    test_cache.set("newk", ap, as);
    if(test_cache.get("newk",blnk) != ap)
    	{std::cout << "Deep copy verified!\n";}
    else
    	{std::cout << "Deep copy failed!\n";}
}

//stores pointer, evicts it, then querries it
std::string basic_evict(Cache::val_type ptr, uint32_t sz)
{
    uint32_t blnk = 0;
    test_cache.set("newk", ptr, sz);
    uint32_t v2 = 2;
    Cache::val_type ptr2 = &v2;
    test_cache.set("key2", ptr2, sizeof(v2));
    std::string outstr = strcast(test_cache.get("newk",blnk));
    std::cout << "Stored:\t" << "Pointer to evict" << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

uint32_t store_evict_store(Cache::val_type ptr, uint32_t sz)
{
    uint32_t outt = 0;
    uint32_t v2 = 10;
    Cache::val_type ptr2 = &v2;
    test_cache.set("newk", ptr, sz);
    outt += test_cache.space_used();
    test_cache.set("key2", ptr2, (sz-1));
    test_cache.set("newk", ptr, sz);
    outt += test_cache.space_used();
    std::cout << "Storing pointer, evicting it, then storing it...\n";
    return outt;
}

uint32_t store_evict_delete(Cache::val_type ptr, uint32_t sz)
{
    uint32_t outt = 0;
    uint32_t v2 = 10;
    Cache::val_type ptr2 = &v2;
    test_cache.set("newk", ptr, sz);
    outt += test_cache.space_used();
    test_cache.set("key2", ptr2, (sz-1));
    outt += test_cache.space_used();
    test_cache.del("newk");
    outt += test_cache.space_used();
    std::cout << "Storing pointer, evicting it, then deleting it...\n";
    return outt;
}

uint32_t basic_size_get(Cache::val_type ptr, uint32_t sz)
{
    uint32_t size_save = 0;
    test_cache.set("newk", ptr, sz);
    test_cache.get("newk", size_save);
    std::cout << "Verifying get returns size...\n";
    return size_save;
}

*/





//Test that may fail: putting in 2 null objects then deleting both