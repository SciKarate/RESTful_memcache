//g++ testing_client.cc cache_murderer.cc -o clm.out -lboost_system -pthread -lcurl -ljsoncpp -Wextra -pedantic -Wall
#include "cache.hh"
#include <iostream>
#include <functional>
#include <unistd.h>

int main()
{
	Cache test_cache(1024);
	unsigned int microseconds;
	microseconds = 500;
	int x = 1;
	while(x < 1000)
	{
		usleep(microseconds);
		std::cout << test_cache.space_used();
		x += 1;
	}
	std::cout << "DONE\n";
	return 0;
}

//Test that may fail: putting in 2 null objects then deleting both