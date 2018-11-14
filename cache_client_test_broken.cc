#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "cache.hh"
#include <iostream>
#include <functional>
//build with "g++ cache.cc cache_tester.cc"

Cache test_cache(1024); //create a cache

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
    test_cache.set("key", ptr, sz);
    std::string outstr = strcast(test_cache.get("key",blnk));
    std::cout << "Stored:\t" << strcast(ptr) << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

//stores pointer, evicts it, then querries it
std::string basic_evict(Cache::val_type ptr, uint32_t sz)
{
    uint32_t blnk = 0;
    test_cache.set("key", ptr, sz);
    uint32_t v2 = 2;
    Cache::val_type ptr2 = &v2;
    test_cache.set("key2", ptr2, sizeof(v2));
    std::string outstr = strcast(test_cache.get("key",blnk));
    std::cout << "Stored:\t" << "Pointer to evict" << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

//stores pointer, deletes, then queries it
std::string basic_delete(Cache::val_type ptr, uint32_t sz)
{
    uint32_t blnk = 0;
    test_cache.set("key", ptr, sz);
    test_cache.del("key");
    std::string outstr = strcast(test_cache.get("key",blnk));
    std::cout << "Stored:\t" << "Pointer to delete" << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

uint32_t store_evict_store(Cache::val_type ptr, uint32_t sz)
{
    uint32_t outt = 0;
    uint32_t v2 = 10;
    Cache::val_type ptr2 = &v2;
    test_cache.set("key", ptr, sz);
    outt += test_cache.space_used();
    test_cache.set("key2", ptr2, (sz-1));
    test_cache.set("key", ptr, sz);
    outt += test_cache.space_used();
    std::cout << "Storing pointer, evicting it, then storing it...\n";
    return outt;
}

uint32_t store_evict_delete(Cache::val_type ptr, uint32_t sz)
{
    uint32_t outt = 0;
    uint32_t v2 = 10;
    Cache::val_type ptr2 = &v2;
    test_cache.set("key", ptr, sz);
    outt += test_cache.space_used();
    test_cache.set("key2", ptr2, (sz-1));
    outt += test_cache.space_used();
    test_cache.del("key");
    outt += test_cache.space_used();
    std::cout << "Storing pointer, evicting it, then deleting it...\n";
    return outt;
}

std::string new_cache_delete()
{
    test_cache.del("key");
    std::string yay = "cache is not broken!";
    return yay;
}

std::string new_cache_get()
{
    uint32_t sz = 0;
    test_cache.get("key",sz);
    std::string yay = "cache is not broken!";
    return yay;
}

uint32_t basic_memused(Cache::val_type ptr, uint32_t sz)
{
    test_cache.set("key", ptr, sz);
    std::cout << "Verifying space_used...\n";
    return test_cache.space_used();
}

uint32_t basic_size_get(Cache::val_type ptr, uint32_t sz)
{
    uint32_t size_save = 0;
    test_cache.set("key", ptr, sz);
    test_cache.get("key", size_save);
    std::cout << "Verifying get returns size...\n";
    return size_save;
}

uint32_t deepcopy(Cache::val_type ptr, uint32_t sz)
{
    uint32_t blnk = 0;
    test_cache.set("key", ptr, sz);
    if(test_cache.get("key",blnk) != ptr)
    {
        std::cout << "Verifying deep copy...\n";
        return true;
    }
    return false;
}

TEST_CASE( "Check int/str get functionality" )
{
    std::string a = "hello";
	Cache::val_type ap = &a;
	int as = sizeof(a);
    
    std::cout << "SET/GET TEST CASES" << std::endl;
    SECTION("the simplest of set/gets")
    {
        REQUIRE(basic_str_set_get(ap,as) == a);
    }
    SECTION("check that memused works")
    {
        REQUIRE(basic_memused(ap,as) == as);
    }
    std::cout << "\n";
    SECTION("check that memused works")
    {
        REQUIRE(basic_size_get(ap,as) == as);
    }
    std::cout << "\n";
}

TEST_CASE( "Check eviction and deletion functionality")
{
    std::string a = "hello";
	Cache::val_type ap = &a;
	int as = sizeof(a);

    std::cout << "DELETION/EVICTION TEST CASES" << std::endl;
    SECTION( "basic eviction" )
    {
        REQUIRE(basic_evict(ap,as) == "");
        std::cout << "\n";
    }
    SECTION( "basic deletion" )
    {
        REQUIRE(basic_delete(ap,as) == "");
        std::cout << "\n";
    }
    SECTION( "store a key, evict it, then store it again")
    {   //check that mem after firt store + mem after third store = 2size
        REQUIRE(store_evict_store(ap,as) == (as * 2));
        std::cout << "\n";
    }
    SECTION( "store a key, evict it, then delete it" )
    {   //check mem after first store + mem after second store + mem after delete
        REQUIRE(store_evict_delete(ap,as) == (as + ((as-1) * 2)));
        std::cout << "\n";
    }
    SECTION(" operations on an empty cache ")
    {
        REQUIRE(new_cache_delete() =="cache is not broken!");
        REQUIRE(new_cache_get() == "cache is not broken!");
        std::cout << "Operating on an empty cache...\n" << std::endl;
    }
}

TEST_CASE( "Checks niche functionality" )
{
	std::string a = "hello";
	Cache::val_type ap = &a;
	int as = sizeof(a);

    std::cout << "NICHE TEST CASES" << std::endl;
    SECTION("test that we are actually deep copying")
    {
        REQUIRE(deepcopy(ap,as) == true);
        std::cout << "\n";
    }
}






//Test that may fail: putting in 2 null objects then deleting both