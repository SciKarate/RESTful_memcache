//g++ testing_client.cc cache_ETC.cc -o cletc.out -lboost_system -pthread -lcurl -ljsoncpp -Wextra -pedantic -Wall
#include "cache.hh"
#include <iostream>
#include <functional>
#include "h_depen/shutcall.hh"
#include <random>
#include <chrono>
#include <ctime>
#include <time.h>
#include <algorithm>

std::string randomString(uint length) //thank you stackexchange
{
	auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

//try running this with 50 if you don't want to wait forever!
int main(int argc, char *argv[])
{
	if(argc < 2)
    {
    	std::cout << "Need more arguments!" << std::endl;
    	std::cout << "Usage: \"./cletc.out [#set/loop]\"" << std::endl;
  		return 0;
    }
    int setamt = atoi(argv[1]);
    int getamt = setamt * 30;
    int delamt = setamt * 6;
    int numremm = getamt+setamt+delamt;
    Cache test_cache(1024);

    //make keys
    int min = 0;
    int max = 0;
    std::string new_key;
    std::string keys[100] = {};
    for (int i = 0; i < 100; i++)
    {
    	int a = 5; //make random 0 to 99
    	int b;
    	if(a < 40)
    	{
    		min = 2; //make random between 2 and 11
    		max = 11;
    	}
    	else if (a < 90)
    	{
    		min = 11; //make random 11 to 128
    		max = 128;
    	}
    	else
    	{
    		min = 128;
    		max = 1024; //make random 128 to 1024
    	}
    	b = min + (rand() % static_cast<int>(max - min + 1));
    	new_key = randomString(b);
    	keys[i] = new_key;
    }

    //run workload
    //get set del
    uint32_t blnk = 0;
    int actchosen = 0;
    int keychosen = 0;
    for (; numremm > 0; numremm--)
    {
    	actchosen = 1 + (rand() % static_cast<int>(numremm)); //make a random between 1 and numremm
    	keychosen = (rand() % static_cast<int>(100)); //make a random between 0 and 99
    	if(actchosen < getamt)
    	{
    		getamt--;
    		test_cache.get(keys[keychosen], blnk);
    	}
    	else if (actchosen < (getamt+setamt))
    	{
    		setamt--;
    		test_cache.set(keys[keychosen],&keys[keychosen],blnk);
    	}
    	else
    	{
    		delamt--;
    		test_cache.del(keys[keychosen]);
    	}
    	numremm = getamt+setamt+delamt;
    	std::cout << numremm << std::endl;
    }
    shutdown_server("localhost","18085");
    return 0;
}