//Josh Reiss and Ryan Neumann
# CS389_HW4

Preface/Dependencies:
	Our server requires:
		-lboost_system, -pthread
	Our clients require:
		-lboost_system, -pthread, -lcurl / libcurl, -ljsoncpp / jsoncpp

Part 1:
	Our server is:
		cache_server.cc
	It can be compiled using the command written as a comment in its first line.

Part 2:
	There are two versions of our cache client.
	One calls a server shutdown in its destructor. The other does not.
	There are also two corresponding versions of our client test.

	The pair of files that shut down the server in the destructor are:
		cache_client_test_shutdown.cc
		testing_client_shutdown.cc

	The pair which do not (our preferred versions) are:
		cache_client_test.cc
		testing_client.cc

	The commands with which to compile these clients are written as comments in their first lines.