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
sudo apt install libcurl4-openssl-dev