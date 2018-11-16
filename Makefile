cache_server: cache_server.cc cache.cc
	g++ -o serv.out cache_server.cc cache.cc -lboost_system -pthread

norm_client: testing_client.cc cache_client_test.cc
	g++ -o cl.out testing_client.cc cache_client_test.cc -lboost_system -pthread -lcurl -ljsoncpp

shutdown_client: testing_client_shutdown.cc cache_client_test_shutdown.cc
	g++ -o cls.out testing_client_shutdown.cc cache_client_test_shutdown.cc -lboost_system -pthread -lcurl -ljsoncpp