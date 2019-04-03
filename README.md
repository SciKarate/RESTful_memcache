#Josh Reiss & Ryan Neumann

#Description
This is a simple, RESTful memcached implementation.
It's a k-v store that takes HTTP requests (POST, GET, PUT, DELETE).
We were given cache.hh by our professor, along with a few API recommendations and the Wikipedia page for networks.
From the API recommendations, we chose Crow (https://github.com/ipkn/crow) because the code seemed very readable.
Below are descriptions of all the files within this repo.

---------------------------------------------------------
#general C Files
cache.cc - implementation of cache structure
testing_client.cc - client implementation
cache_server.cc - server implementation

---------------------------------------------------------
#testing C files
cache_client_test.cc	- defines test cases for cache client
cache_ETC.cc 			- blasts the server with a test client, represents Facebook's ETC workload from a paper we read
cache_load.cc 			- blasts server with random requests; returns information abt how it performed
cache_respsize.cc 		- tests how response time and request size relate
cache_resptimer.cc 		- tests how response time and requests/sec relate

---------------------------------------------------------
#.out descriptions
serv.out 	- server implementation 	- (see cache_server.cc)
cls.out 	- runs basic client tests 	- (see cache_client_test.cc)
cletc.out 	- runs ETC workload 		- (see cache_ETC.cc)
cload.out 	- random request workload 	- (see cache_load.cc)
clrsize.out - tests resp size 			- (see cache_respsize.cc)
clresp.out 	- tests req rate 			- (see cache_resptimer.cc)

---------------------------------------------------------
#compilation instructions
serv.out 	- g++ cache_server.cc cache.cc -o serv.out -lboost_system -pthread
cls.out 	- g++ testing_client.cc cache_client_test.cc -o cls.out -lboost_system -pthread -lcurl -ljsoncpp
cletc.out 	- g++ testing_client.cc cache_ETC.cc -o cletc.out -lboost_system -pthread -lcurl -ljsoncpp
cload.out 	- g++ testing_client.cc cache_load.cc -o cload.out -lboost_system -pthread -lcurl -ljsoncpp
clrsize.out - g++ testing_client.cc cache_respsize.cc -o clrsize.out -lboost_system -pthread -lcurl -ljsoncpp
clresp.out 	- g++ testing_client.cc cache_resptimer.cc -o clresp.out -lboost_system -pthread -lcurl -ljsoncpp
			(ought to add -Wextra -pedantic -Wall to all of these)

---------------------------------------------------------
#package download commands (Ubuntu)
sudo apt-get install libjsoncpp-dev
sudo apt-get install libpthread-stubs0-dev
sudo apt install libboost-all-dev
<<<<<<< HEAD
sudo apt install libcurl4-openssl-dev
=======
sudo apt install libcurl4-openssl-dev

//This project is a mess, but it works!!
//The documentation is weird and there's a billion files because the scope changed every week. Sorry!



HW6 - Step 1
After HW5, our response time was already below 1ms for GETs at higher than 10 requests per second.

It is worth noting that all our tests were run on localhost.
One could argue that, although this removes overhead caused by Reed's network, it introduces new overhead in that the server and cache might be stealing cycles from eachother.
We have disproved this theory via experiment. On a machine with a 16-thread 4.6GHz CPU and an 8-thread 4.0GHz CPU, there is no difference in performance.
If there were cycles being stolen on the 8-thread 4.0GHz CPU, one would expect a change in performance on the CPU with a higher clock speed and more threads.
Therefore, We believe that we are not encountering any problems with cycle theft.







HW6 - Step 2
On Josh's laptop, we reach the saturation point of 1ms at 277 requests per second with the described workload.

On Ryan's laptop, we reach the saturation point of 1ms at 242 requests per second with the described workload.

Running perf shows -
14.04% overhead for cload.out from [kernel.kallsysms] syscall_return_via_sysret
11.42% overhead for cload.out from [unknown] 0xfffffe000000601b
All other overhead is very minimal.
Surprisingly, our server's overhead never exceeds 0.03% for any item.

Before identifying bottlenecks, we used crow's built-in multithreading.
This means our server is now multithreaded.
Then, to identify these bottlenecks, we tried to see if the following affected their overhead percentages:
-> Removing all random generation, to no effect.
-> Using only one key and one value, to no effect.

We cannot think of what else would be using 11.42% overhead at a seemingly arbitrary memory address.





Odd Note:
	cload.out and cletc.out both segfault after finishing on Ryan's computer every time, but not on Josh's. We suspect that this might be caused by our random string generation, as clresp.out does not segfault and the only substantial differentiator between clresp and cletc/cload is random string generation.
	cload.out and cletc.out leak memory all over the place, but we can't identify the cause for that. We never malloc or new anywhere.




Related files:
	cache_load.cc
	cache_server.cc
	cload.out
	serv.out
	perf.data
>>>>>>> master
