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

//all tests must be successive because the deconstructor
//shuts down the server in this version
TEST_CASE("test shutdown version functionality")
{
    //initializing tests
    Cache test_cache(100);
    std::string a = "hello";
    Cache::val_type ap = &a;
    int as = sizeof(a);
    uint32_t blnk = 0;
    std::string outstr;

//new cache delete test
    REQUIRE(test_cache.del("newk") == 0);

//new cache get test
    outstr = strcast(test_cache.get("newk",blnk)); 
    REQUIRE(outstr == "NULL");

//basic set get test
    test_cache.set("newk", ap, as);
    outstr = strcast(test_cache.get("newk",blnk));
    REQUIRE(outstr == "hello");

//basic delete test
    test_cache.del("newk");
    outstr = strcast(test_cache.get("newk",blnk));
    REQUIRE(outstr == "NULL");

//basic memused test
    test_cache.set("newk", ap, as);
    REQUIRE(test_cache.space_used()==6);

//basic deep copy test
    REQUIRE(test_cache.get("newk",blnk) != ap);
}





/*

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