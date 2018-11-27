//g++ testing_client.cc cache_resptimer.cc -o clgodhelp.out -lboost_system -pthread -lcurl -ljsoncpp -Wextra -pedantic -Wall
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
	if(argc < 2)
    {
    	std::cout << "Need more arguments!" << std::endl;
    	std::cout << "Usage: \"./cl.out [# reqs/sec] [# total reqs]\"" << std::endl;
  		return 0;
    }
    int reqssec = atoi(argv[1]);
    int totalreqs = atoi(argv[2]);
    Cache test_cache(1024);
    double wait_time = 1000000000 / reqssec;
    int reqsthus = 0;
    int oldreq = reqsthus;
    long ss;
    long ns;
    double time_elapsed;
    double time_elapsed_noget;
    uint32_t bleep = 0;

    struct timespec start, currtime, prevtime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    prevtime.tv_sec = start.tv_sec;
    prevtime.tv_nsec = start.tv_nsec;

    while(reqsthus < totalreqs)
    {
    	test_cache.get("hello", bleep);
    	clock_gettime(CLOCK_MONOTONIC_RAW, &currtime);

    	while(oldreq == reqsthus)
    	{
    		clock_gettime(CLOCK_MONOTONIC_RAW, &currtime);
    		ss = (currtime.tv_sec - prevtime.tv_sec);
    		ns = (currtime.tv_nsec - prevtime.tv_nsec);
    		time_elapsed = ((double)ss * 1000000000) + (double)ns;
    		if(time_elapsed >= wait_time)
    		{
    			reqsthus += 1;
    		}
    	}
    	prevtime.tv_sec = currtime.tv_sec;
    	prevtime.tv_nsec = currtime.tv_nsec;
    	oldreq = reqsthus;
    }
    ss = (currtime.tv_sec - start.tv_sec);
    ns = (currtime.tv_nsec - start.tv_nsec);
    time_elapsed = ((double)ss * 1000000000) + (double)ns;
    time_elapsed = time_elapsed - (wait_time * totalreqs);
    //time_elapsed = time_elapsed - (time_elapsed_noget);
    double avg_time = time_elapsed / (totalreqs);
    avg_time = avg_time / 1000000;
    //shutdown_server("localhost","18085");
    printf("Reponse time in ms:\t%fms\n", avg_time);
    return 0;
}

//Test that may fail: putting in 2 null objects then deleting both