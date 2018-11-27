//g++ testing_client.cc cache_resptimer.cc -o cletc.out -lboost_system -pthread -lcurl -ljsoncpp -Wextra -pedantic -Wall
#include "cache.hh"
#include <iostream>
#include <functional>
#include "h_depen/shutcall.hh"
#include <random>
#include <chrono>
#include <ctime>
#include <time.h>

std::string randomString(uint l)
{
	std::string charIndex = "abcdefghijklmnaoqrstuvwxyz";
	uint length = rand() % l + 1;
	uint ri[1024];
	for (uint i = 0; i < length; i++)
	{
		ri[i] = rand % charIndex.length();
	}
	std::string rs = "";
	for (uint i = 0; i < length; i++)
	{
		rs += charIndex[ri[i]];
	}
	if(rs.empty() randomString(l));
	else return rs;
}

//gets to updates/sets is 30:1
//40% of requests are 2 - 11 bytes

int main(int argc, char *argv[])
{
	if(argc < 1)
    {
    	std::cout << "Need more arguments!" << std::endl;
    	std::cout << "Usage: \"./cletc.out [#set/loop]\"" << std::endl;
  		return 0;
    }
    int setamt = atoi(argv[1]);
    int getamt = setamt * 30;
    int delamt = setamt * 7;
    Cache test_cache(1024);
    std::string keys[100] = {};
    for (int i = 0; i < 100, i++)
    {
    	int le = 5; //make random
    	new_key = randomString(le);
    	keys[i] = new_key;
    }

    return 0;
}