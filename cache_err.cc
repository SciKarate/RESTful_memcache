#include "cache.hh"
#include <iostream>
#include <functional>

int main()
{
	uint32_t sz = 0;
    int cache_size = 64;
    Cache server_cache(cache_size);
    std::cout << server_cache.space_used();
   	std::string a = "6";
   	Cache::val_type aptr = &a;
    server_cache.set("hey",aptr,sizeof(a));
    Cache::val_type bptr = server_cache.get("hey",sz);
    std::cout << *((std::string*)bptr) << std::endl;
    std::cout << *((int*)bptr) << std::endl;
}