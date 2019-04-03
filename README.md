# Josh Reiss & Ryan Neumann - RESTful_memcached

## Description<br>
This is a simple, RESTful memcached implementation.<br>
It's a k-v store that takes HTTP requests (POST, GET, PUT, DELETE).<br>
We were given cache.hh by our professor, along with a few API recommendations and the Wikipedia page for networks.<br>
From the API recommendations, we chose Crow (https://github.com/ipkn/crow) because the code seemed very readable.<br>
Below are descriptions of all the files within this repo.<br>

---------------------------------------------------------
## general C Files<br>
cache.cc - implementation of cache structure<br>
testing_client.cc - client implementation<br>
cache_server.cc - server implementation<br>

---------------------------------------------------------
## testing C files<br>
cache_client_test.cc	- defines test cases for cache client<br>
cache_ETC.cc 			- blasts the server with a test client, represents Facebook's ETC workload from a paper we read<br>
cache_load.cc 			- blasts server with random requests; returns information abt how it performed<br>
cache_respsize.cc 		- tests how response time and request size relate<br>
cache_resptimer.cc 		- tests how response time and requests/sec relate<br>

---------------------------------------------------------
## .out descriptions<br>
serv.out 	- server implementation 	- (see cache_server.cc)<br>
cls.out 	- runs basic client tests 	- (see cache_client_test.cc)<br>
cletc.out 	- runs ETC workload 		- (see cache_ETC.cc)<br>
cload.out 	- random request workload 	- (see cache_load.cc)<br>
clrsize.out - tests resp size 			- (see cache_respsize.cc)<br>
clresp.out 	- tests req rate 			- (see cache_resptimer.cc)<br>

---------------------------------------------------------
## compilation instructions<br>
serv.out 	- g++ cache_server.cc cache.cc -o serv.out -lboost_system -pthread<br>
cls.out 	- g++ testing_client.cc cache_client_test.cc -o cls.out -lboost_system -pthread -lcurl -ljsoncpp<br>
cletc.out 	- g++ testing_client.cc cache_ETC.cc -o cletc.out -lboost_system -pthread -lcurl -ljsoncpp<br>
cload.out 	- g++ testing_client.cc cache_load.cc -o cload.out -lboost_system -pthread -lcurl -ljsoncpp<br>
clrsize.out - g++ testing_client.cc cache_respsize.cc -o clrsize.out -lboost_system -pthread -lcurl -ljsoncpp<br>
clresp.out 	- g++ testing_client.cc cache_resptimer.cc -o clresp.out -lboost_system -pthread -lcurl -ljsoncpp<br>
			(ought to add -Wextra -pedantic -Wall to all of these)<br>

---------------------------------------------------------
## package download commands (Ubuntu)<br>
sudo apt-get install libjsoncpp-dev<br>
sudo apt-get install libpthread-stubs0-dev<br>
sudo apt install libboost-all-dev<br>
sudo apt install libcurl4-openssl-dev<br>
