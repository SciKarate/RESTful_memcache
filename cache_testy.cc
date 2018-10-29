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
//Cache size, a ptr, bptr, flusher, a size, b size, flusher size
uint32_t cache_test_cacheflush(int casz, Cache::val_type aptr, Cache::val_type bptr, Cache::val_type fptr, int asz, int bsz, int fsz)
{
    using namespace std;
    cout << "\nStoring two pointers that fit, one that's bigger than the cache\nThen, verifying that we don't evict the first two..." << endl;
    Cache test_cache(casz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("a_key", aptr, asz);
    cout << "Storing first pointer..." << "\t" << "it's size is\t" << asz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;

    test_cache.set("b_key", bptr, bsz);
    cout << "Storing second pointer..." << "\t" << "it's size is\t" << bsz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;

    test_cache.set("f_key", fptr, fsz);
    cout << "Storing flush pointer..." << "\t" << "it's size is\t" << fsz << "\t" << "& space_used is\t" << test_cache.space_used() <<endl;
    return test_cache.space_used();
}

//Cache size, a ptr, bptr, cptr, flusher, a size, b size, c size
uint32_t cache_test_samekey(int casz, Cache::val_type aptr, Cache::val_type bptr, Cache::val_type cptr, int asz, int bsz, int csz)
{
    uint32_t sz;
    uint32_t size_sum = 0;
    using namespace std;
    cout << "\nStoring three pointers at the same key\nThen, verifying that each of them affects memused_ correctly..." << endl;
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

uint32_t basic_int_set_get(Cache::val_type ptr, uint32_t sz)
{
	Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
	test_cache.set("key", ptr, sz);
	uint32_t outint = intcast(test_cache.get("key",sz));
    std::cout << "\nStored:\t" << intcast(ptr) << "\tRetrieved:\t" << outint << std::endl;
    return outint;
}

std::string basic_str_set_get(Cache::val_type ptr, uint32_t sz)
{
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("key", ptr, sz);
    std::string outstr = strcast(test_cache.get("key",sz));
    std::cout << "Stored:\t" << strcast(ptr) << "\tRetrieved:\t" << outstr << std::endl;
    return outstr;
}

uint32_t basic_evict(Cache::val_type ptr, uint32_t sz)
{
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("key", ptr, sz);
    uint32_t v2 = 10;
    Cache::val_type ptr2 = &v2;
    test_cache.set("key2", ptr2, sizeof(v2));
    uint32_t outint = intcast(test_cache.get("key",sz));
    std::cout << "Stored:\t" << "Pointer to evict" << "\tRetrieved:\t" << outint << std::endl;
    return outint;
}

uint32_t basic_delete(Cache::val_type ptr, uint32_t sz)
{
    Cache test_cache(sz, [](){return 0;}, my_hash_func); //create a cache
    test_cache.set("key", ptr, sz);
    test_cache.del("key");
    uint32_t outint = intcast(test_cache.get("key",sz));
    std::cout << "Stored:\t" << "Pointer to delete" << "\tRetrieved:\t" << outint << std::endl;
    return outint;
}

TEST_CASE( "Factorials are computed" ) {

	int a = 12;
	std::string b = "hello";
	int f[10] = {1,2,3,4,5,6,7,8,9,10};
	Cache::val_type ap = &a;
	Cache::val_type bp = &b;
	Cache::val_type fp = &f;
	int as = sizeof(a); int bs = sizeof(b); int fs = sizeof(f);
    REQUIRE(basic_int_set_get(ap,as) == a);
    REQUIRE(basic_str_set_get(bp,bs) == b);
    std::cout << "\n";
    REQUIRE(basic_evict(ap,as) == (0));
    REQUIRE(basic_evict(bp,bs) == (0));
    REQUIRE(basic_evict(fp,fs) == (0));
    std::cout << "\n";
    REQUIRE(basic_delete(ap,as) == (0));
    REQUIRE(basic_delete(bp,bs) == (0));
    REQUIRE(basic_delete(fp,fs) == (0));
    REQUIRE(cache_test_cacheflush((as+bs+1), ap, bp, fp, as, bs, fs) == as+bs);
    REQUIRE(cache_test_samekey((as+bs+fs), ap, bp, fp, as, bs, fs) == as+bs+fs);
}