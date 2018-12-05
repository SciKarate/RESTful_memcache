//g++ testing_client.cc cache_resptimer.cc -o clresp.out -lboost_system -pthread -lcurl -ljsoncpp -Wextra -pedantic -Wall
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

	int ngreqsthus = 0;
    int ngoldreq = ngreqsthus;
    long ngss;
    long ngns;
    double ngtime_elapsed;

    int reqsthus = 0;
    int oldreq = reqsthus;
    long ss;
    long ns;
    double time_elapsed;
    uint32_t bleep = 0;

//timing the overhead (no server request, ng = no get)
    uint32_t first = 0;
    struct timespec ngstart, ngcurrtime, ngprevtime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ngstart);
    ngprevtime.tv_sec = ngstart.tv_sec;
    ngprevtime.tv_nsec = ngstart.tv_nsec;

    while(ngreqsthus < totalreqs)
    {

    	while(ngoldreq == ngreqsthus)
    	{
    		if(first == 0)
    		{
    			first = 1;
    			clock_gettime(CLOCK_MONOTONIC_RAW, &ngstart);
    		}
    		clock_gettime(CLOCK_MONOTONIC_RAW, &ngcurrtime);
    		ngss = (ngcurrtime.tv_sec - ngprevtime.tv_sec);
    		ngns = (ngcurrtime.tv_nsec - ngprevtime.tv_nsec);
    		ngtime_elapsed = ((double)ngss * 1000000000) + (double)ngns;
    		if(ngtime_elapsed >= wait_time)
    		{
    			ngreqsthus += 1;
    		}
    	}
    	ngprevtime.tv_sec = ngcurrtime.tv_sec;
    	ngprevtime.tv_nsec = ngcurrtime.tv_nsec;
    	ngoldreq = ngreqsthus;
    }

    ngss = (ngcurrtime.tv_sec - ngstart.tv_sec);
    ngns = (ngcurrtime.tv_nsec - ngstart.tv_nsec);
    ngtime_elapsed = ((double)ngss * 1000000000) + (double)ngns;
    ngtime_elapsed = ngtime_elapsed - (wait_time * totalreqs);
    double ngavg_time = ngtime_elapsed / (totalreqs);
    ngavg_time = ngavg_time / 1000000;//conversion to ms



//timing both overhead and server response time
    struct timespec start, currtime, prevtime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    prevtime.tv_sec = start.tv_sec;
    prevtime.tv_nsec = start.tv_nsec;
    first = 0;

    while(reqsthus < totalreqs)
    {
    	test_cache.get("hello", bleep);

    	while(oldreq == reqsthus)
    	{
    		if(first == 0)
    		{
    			first = 1;
    			clock_gettime(CLOCK_MONOTONIC_RAW, &ngstart);
    		}
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
    time_elapsed = time_elapsed - (wait_time * totalreqs);//subtracting wait time to measure response time explicitly
    //time_elapsed = time_elapsed - (time_elapsed_noget);
    double avg_time = time_elapsed / (totalreqs);
    avg_time = avg_time / 1000000;
    //subtracting overhead from response time + overhead
    double responsetime = time_elapsed - ngtime_elapsed;
    double avg_response_time = responsetime / (totalreqs);
    avg_response_time = avg_response_time/1000000;

    //shutdown_server("localhost","18085");
    printf("total overhead time elapsed:\t%fss\n", ngtime_elapsed);
    printf("total response time elapsed:\t%fss\n", time_elapsed);
    printf("Reponse time with overhead in ms:\t%fms\n", avg_time);
    printf("Reponse time without overhead in ms:\t%fms\n", avg_response_time);
    shutdown_server("localhost","18085");
    return 0;
}
/*Response time appears to increase exponentially while overhead scales
proportionally with requests/sec. This renders overhead irrelevant 
except at low request rates. At the requests/second resulting in the 
desired response time of just under a ms, overhead only delays response 
time by about 1/10,000th.
*/

//246 = max requests per second resulting in <1 ms response time on RyanMacBook
//245 = max requests per second resulting in <1 ms response time on JoshRzBlade