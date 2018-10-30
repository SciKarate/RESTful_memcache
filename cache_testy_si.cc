#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include "cache_si.hh"
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
//Cache size, a ptr, bptr, flusher, a size, b size, flusher size
uint32_t cache_test_flush(Cache::val_type aptr, Cache::val_type bptr, Cache::val_type fptr, int asz, int bsz, int fsz)
{
    using namespace std;
    uint32_t casz = asz+bsz+1;
    cout << "Storing two pointers that fit, one that's bigger than the cache\nThen, verifying that we don't evict the first two..." << endl;
    Cache test_cache(casz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("a_key", aptr, asz);
    cout << "Storing first pointer..." << "\t" << "its size is\t" << asz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;

    test_cache.set("b_key", bptr, bsz);
    cout << "Storing second pointer..." << "\t" << "its size is\t" << bsz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;

    test_cache.set("f_key", fptr, fsz);
    cout << "Storing flush pointer..." << "\t" << "its size is\t" << fsz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;
    return test_cache.space_used();
}

//Cache size, a ptr, bptr, cptr, flusher, a size, b size, c size
uint32_t cache_test_samekey(Cache::val_type aptr, Cache::val_type bptr, Cache::val_type cptr, int asz, int bsz, int csz)
{
    uint32_t sz;
    uint32_t size_sum = 0;
    using namespace std;
    uint32_t casz = 0;
    if (casz < asz) {casz = asz;}
    if (casz < bsz) {casz = bsz;}
    if (casz < csz) {casz = csz;}
    casz += 1;
    cout << "Storing three pointers at the same key\nThen, verifying that each of them affects memused_ correctly..." << endl;
    Cache test_cache(casz, [](){return 0;}, my_hash_func); //create a cache

    test_cache.set("rep_key", aptr, asz);
    cout << "Storing first pointer..." << "\t" << "it's size is\t" << asz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;
    size_sum += test_cache.space_used();

    test_cache.set("rep_key", bptr, bsz);
    cout << "Storing second pointer..." << "\t" << "it's size is\t" << bsz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;
    size_sum += test_cache.space_used();

    test_cache.set("rep_key", cptr, csz);

    cout << "Storing third pointer..." << "\t" << "it's size is\t" << csz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;
    size_sum += test_cache.space_used();
    return size_sum;
}

//stores int pointer, then querries it
uint32_t basic_int_set_get(Cache::val_type ptr, uint32_t sz)
{
	Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
	test_cache.set("key", ptr, sz);
	uint32_t outint = intcast(test_cache.get("key"));
    std::cout << "Stored:\t" << intcast(ptr) << "\tRetrieved:\t" << outint << std::endl;
    return outint;
}

//stores string pointer, then querries it
std::string basic_str_set_get(Cache::val_type ptr, uint32_t sz)
{
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("key", ptr, sz);
    std::string outstr = strcast(test_cache.get("key"));
    std::cout << "Stored:\t" << strcast(ptr) << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

//stores pointer, evicts it, then querries it
uint32_t basic_evict(Cache::val_type ptr, uint32_t sz)
{
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("key", ptr, sz);
    uint32_t v2 = 2;
    Cache::val_type ptr2 = &v2;
    test_cache.set("key2", ptr2, sizeof(v2));
    uint32_t outint = intcast(test_cache.get("key"));
    std::cout << "Stored:\t" << "Pointer to evict" << "\tRetrieved:\t" << outint << std::endl;
    return outint;
}

//stores pointer, deletes, then queries it
uint32_t basic_delete(Cache::val_type ptr, uint32_t sz)
{
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("key", ptr, sz);
    test_cache.del("key");
    uint32_t outint = intcast(test_cache.get("key"));
    std::cout << "Stored:\t" << "Pointer to delete" << "\tRetrieved:\t" << outint << std::endl;
    return outint;
}

uint32_t store_evict_store(Cache::val_type ptr, uint32_t sz)
{
    uint32_t outt = 0;
    uint32_t v2 = 10;
    Cache::val_type ptr2 = &v2;
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
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
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
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
    Cache test_cache(4, [](){return 0;}, my_hash_func); //create a cache
    test_cache.del("key");
    std::string yay = "cache is not broken!";
    return yay;
}

std::string new_cache_get()
{
    uint32_t sz = 0;
    Cache test_cache(4, [](){return 0;}, my_hash_func); //create a cache
    test_cache.get("key");
    std::string yay = "cache is not broken!";
    return yay;
}

uint32_t basic_memused(Cache::val_type ptr, uint32_t sz)
{
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("key", ptr, sz);
    std::cout << "Verifying space_used...\n";
    return test_cache.space_used();
}

TEST_CASE( "Check int/str get functionality" )
{
    int a = 12;
    std::string b = "hello";
    int f[10] = {1,2,3,4,5,6,7,8,9,10};
    Cache::val_type ap = &a;
    Cache::val_type bp = &b;
    Cache::val_type fp = &f;
    int as = sizeof(a); int bs = sizeof(b); int fs = sizeof(f);
    
    std::cout << "SET/GET TEST CASES" << std::endl;
    SECTION("the simplest of set/gets")
    {
        REQUIRE(basic_int_set_get(ap,as) == a);
        REQUIRE(basic_str_set_get(bp,bs) == b);
    }
    SECTION("check that memused works")
    {
        REQUIRE(basic_memused(ap,as) == as);
        REQUIRE(basic_memused(bp,bs) == bs);
        REQUIRE(basic_memused(fp,fs) == fs);
    }
    std::cout << "\n";
}

TEST_CASE( "Check eviction and deletion functionality")
{
    int a = 12;
    std::string b = "hello";
    int f[10] = {1,2,3,4,5,6,7,8,9,10};
    Cache::val_type ap = &a;
    Cache::val_type bp = &b;
    Cache::val_type fp = &f;
    int as = sizeof(a); int bs = sizeof(b); int fs = sizeof(f);

    std::cout << "DELETION/EVICTION TEST CASES" << std::endl;
    SECTION( "basic eviction" )
    {
        REQUIRE(basic_evict(ap,as) == (0));
        REQUIRE(basic_evict(bp,bs) == (0));
        REQUIRE(basic_evict(fp,fs) == (0));
        std::cout << "\n";
    }
    SECTION( "basic deletion" )
    {
        REQUIRE(basic_delete(ap,as) == (0));
        REQUIRE(basic_delete(bp,bs) == (0));
        REQUIRE(basic_delete(fp,fs) == (0));
        std::cout << "\n";
    }
    SECTION( "store a key, evict it, then store it again")
    {   //check that mem after firt store + mem after third store = 2size
        REQUIRE(store_evict_store(ap,as) == (as * 2));
        REQUIRE(store_evict_store(bp,bs) == (bs * 2));
        REQUIRE(store_evict_store(fp,fs) == (fs * 2));
        std::cout << "\n";
    }
    SECTION( "store a key, evict it, then delete it" )
    {   //check mem after first store + mem after second store + mem after delete
        REQUIRE(store_evict_delete(ap,as) == (as + ((as-1) * 2)));
        REQUIRE(store_evict_delete(bp,bs) == (bs + ((bs-1) * 2)));
        REQUIRE(store_evict_delete(fp,fs) == (fs + ((fs-1) * 2)));
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
	int a = 12;
	std::string b = "hello";
	int f[10] = {1,2,3,4,5,6,7,8,9,10};
	Cache::val_type ap = &a;
	Cache::val_type bp = &b;
	Cache::val_type fp = &f;
	int as = sizeof(a); int bs = sizeof(b); int fs = sizeof(f);

    std::cout << "NICHE TEST CASES" << std::endl;
    SECTION( "test what happens when we store a key bigger than cache")
    {
        REQUIRE(cache_test_flush(ap, bp, fp, as, bs, fs) == as+bs);
        std::cout << "\n";
    }
    SECTION( "test various type assignment to same key" )
    {
        REQUIRE(cache_test_samekey(ap, bp, fp, as, bs, fs) == as+bs+fs);
        std::cout << "\n";
    }
}