//g++ testing_client.cc cache_workload_test.cc -o clworkload.out -lboost_system -pthread -lcurl -ljsoncpp -Wextra -pedantic -Wall
#include "cache.hh"
#include <iostream>
#include <functional>
#include "h_depen/shutcall.hh"
#include <random>
#include <chrono>
#include <ctime>
#include <time.h>

int main(int argc, char *argv[])
{
	if(argc < 1)
    {
    	std::cout << "Need more arguments!" << std::endl;
    	std::cout << "Usage: \"./cl.out [runtime in seconds]\"" << std::endl;
  		return 0;
    }

    double time;
    double runtime = atoi(argv[1]);
    runtime = runtime*1000000000;
	double wait_time = 1000/15; //roughly 240 reqs/second if 16 requests per loop
	long ss;
    long ns;
    double time_elapsed;

    //initializing items for cache
    Cache test_cache(1024);
    uint32_t sz = 0;
    std::string a = "hello";
    Cache::val_type ap = &a;
    int as = sizeof(a);


	struct timespec start, currtime, prevtime;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    prevtime.tv_sec = start.tv_sec;
    prevtime.tv_nsec = start.tv_nsec;
    while (time < runtime)
    {
	    
    	
		//simulating workload (16 requests)
		test_cache.set("key", ap, as);

		test_cache.get("key",sz);
		test_cache.get("badkey",sz);
		test_cache.get("key",sz);
		test_cache.get("key",sz);
		test_cache.get("key",sz);
		test_cache.get("key",sz);
		test_cache.get("badkey",sz);
		test_cache.get("key",sz);
		test_cache.get("key",sz);

		test_cache.del("badkey");
		test_cache.del("key");
		test_cache.del("key");
		test_cache.del("key");
		test_cache.del("key");
		test_cache.del("key");


		
		while (time_elapsed <= wait_time)//delays loop to achieve ~240 reqs/sec
	    {
	    	clock_gettime(CLOCK_MONOTONIC_RAW, &currtime);
	    	ss = (currtime.tv_sec - prevtime.tv_sec);
			ns = (currtime.tv_nsec - prevtime.tv_nsec);
			time_elapsed = ((double)ss * 1000000000) + (double)ns;
			time += time_elapsed;
			std::cout<<"delay"<<std::endl;
	    }
	    time_elapsed = 0;//resetting loop
	}





}