//g++ testing_client.cc cache_client_test.cc -o cl.out -lboost_system -pthread -lcurl -ljsoncpp -Wextra -pedantic -Wall
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "h_depen/catch.hpp"
#include "cache.hh"
#include <iostream>
#include <functional>
#include "h_depen/shutcall.hh"

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
	Cache test_cache(1024);
    uint32_t blnk = 0;
    test_cache.set("key", ptr, sz);
    std::string outstr = strcast(test_cache.get("key",blnk));
    std::cout << "Stored:\t" << strcast(ptr) << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

//stores pointer, deletes, then queries it
std::string basic_delete(Cache::val_type ptr, uint32_t sz)
{
	Cache test_cache(1024);
    uint32_t blnk = 0;
    test_cache.set("key", ptr, sz);
    test_cache.del("key");
    std::string outstr = strcast(test_cache.get("key",blnk));
    std::cout << "Stored:\t" << "Pointer to delete" << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

std::string new_cache_delete()
{
	Cache test_cache(1024);
    test_cache.del("key");
    std::string yay = "cache is not broken!";
    return yay;
}

std::string new_cache_get()
{
	Cache test_cache(1024);
    uint32_t sz = 0;
    test_cache.get("key",sz);
    std::string yay = "cache is not broken!";
    return yay;
}

uint32_t basic_memused(Cache::val_type ptr, std::string pk)
{
	Cache test_cache(1024);
	uint32_t sz = 0;
	std::string *vptr = new std::string(pk);
	const auto& str = *static_cast<const std::string*>(vptr);
	int val_size = str.size() + 1;
	delete(vptr);

    test_cache.set("key", ptr, sz);
    std::cout << "Verifying space_used...\n";
    return (test_cache.space_used() - val_size);
}

uint32_t basic_size_get(Cache::val_type ptr, std::string pk)
{
	Cache test_cache(1024);
    uint32_t size_save = 0;
    std::string *vptr = new std::string(pk);
	const auto& str = *static_cast<const std::string*>(vptr);
	int val_size = str.size() + 1;
	delete(vptr);

    test_cache.set("key", ptr, size_save);
    test_cache.get("key", size_save);
    std::cout << "Verifying get returns size...\n";
    return (size_save - val_size);
}

uint32_t deepcopy(Cache::val_type ptr, uint32_t sz)
{
	Cache test_cache(1024);
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
        REQUIRE(basic_memused(ap,a) == 0);
    }
    SECTION("check that memused works")
    {
        REQUIRE(basic_size_get(ap,a) == 0);
    }
    std::cout << "\n";
}

TEST_CASE( "Check eviction and deletion functionality")
{
    std::string a = "hello";
	Cache::val_type ap = &a;
	int as = sizeof(a);

    std::cout << "DELETION/EVICTION TEST CASES" << std::endl;
    SECTION( "basic deletion" )
    {
        REQUIRE(basic_delete(ap,as) == "NULL");
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
    shutdown_server("localhost","18085");
}






//Test that may fail: putting in 2 null objects then deleting both