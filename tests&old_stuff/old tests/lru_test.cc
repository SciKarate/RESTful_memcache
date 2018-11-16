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

int main()
{
	Cache test_cache(19, [](){return 0;}, my_hash_func); //create a cache
	int a = 10;
	int b = 20;
	int c = 30;
	int d = 40;
	int e = 50;
	Cache::val_type ap = &a;
	Cache::val_type bp = &b;
	Cache::val_type cp = &c;
	Cache::val_type dp = &d;
	Cache::val_type ep = &e;
	uint32_t as = sizeof(a);
	uint32_t bs = sizeof(b);
	uint32_t cs = sizeof(c);
	uint32_t ds = sizeof(d);
	uint32_t es = sizeof(e);
	test_cache.set("a",ap,as);
	test_cache.set("b",bp,bs);
	test_cache.set("c",cp,cs);
	test_cache.set("d",dp,ds);
	test_cache.set("e",ep,es);
	test_cache.get("a",as);
	test_cache.get("d",cs);
	test_cache.get("c",cs);
	test_cache.get("e",cs);
	test_cache.get("c",cs);
	return 0;
}
//boop