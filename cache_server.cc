//g++ cache_server.cc cache.cc -lboost_system -pthread
//needs: boost_system
#include "crow.h"
#include "cache.hh"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <stdio.h>
using val_t = std::string;

int main(int argc, char *argv[])
{
	crow::SimpleApp app;
	if(argc < 2)
    {
    	std::cout << "Need more arguments!" << std::endl;
    	std::cout << "Usage: \"./server.exe maxmem port\"" << std::endl;
  		return 0;
    }
    int portsaved = atoi(argv[2]);
    int cache_size = atoi(argv[1]);
	Cache server_cache(cache_size);


	CROW_ROUTE(app, "/shutdown").methods("POST"_method)
	([&server_cache, &app](const crow::request& req)
	{
		if (req.method == "POST"_method)
		{
			app.stop();
			return crow::response(200);	
		}
		else
			{return crow::response(404);}
	});


    CROW_ROUTE(app, "/memsize").methods("GET"_method)
	([&server_cache, cache_size](const crow::request& req)
	{
		if (req.method == "GET"_method)
		{
			std::ostringstream os;
			os << "{ \"memused\": \"";
	    	os << server_cache.space_used();
		    os << "\", \"maxmem\": \"";
		    os << cache_size;
		    os << "\" }";
		    os << std::endl;
			return crow::response{os.str()};
		}
		else
			{return crow::response(404);}
	});


	CROW_ROUTE(app, "/key/<string>/<string>").methods("PUT"_method)
	([&server_cache](const crow::request& req, std::string k, std::string v)
	{
		if (req.method == "PUT"_method)
		{
			std::string *vptr = new std::string(v);
			const auto& str = *static_cast<const std::string*>(vptr);
			uint32_t sz = server_cache.set(k, str.c_str(), str.size() + 1);
			delete vptr;

			if (sz == 0)
				{return crow::response(200);}
			else
				{return crow::response(400);}
		}
		else
			{return crow::response(404);}
	});


	CROW_ROUTE(app, "/key/<str>")
	.methods("HEAD"_method, "GET"_method, "DELETE"_method)
	([&server_cache](const crow::request& req, std::string k)
	{
    	if (req.method == "HEAD"_method)
    	{
    		char timestr[50];
			time_t now = time(0);
			struct tm tm = *gmtime(&now);
			strftime(timestr, sizeof(timestr), "%a, %d %b %Y %H:%M:%S %Z", &tm);
    		
    		std::ostringstream os;
    		os << "{ ";
    		os << "\"Accept\": \"text/html\", ";
    		os << "\"Content-Type\": \"application/x-www-form-urlencoded\", ";
    		os << "\"Date\": \"" << timestr << "\", ";
    		os << "\"HTTP version\": \"";
    		os << http_parser_version();
    		os << "\" }";
	    	os << std::endl;
			return crow::response{os.str()};
    	}

    	else if (req.method == "GET"_method)
    	{
    		uint32_t sz = 0;
    		Cache::val_type v = server_cache.get(k, sz);
    		std::ostringstream os;
    		os << "{ \"key\": \"" << k << "\", \"value\": \"";
    		if(v != NULL)
    			{os << static_cast<const char*>(v);}
    		else
	    		{os << "NULL";}
    		os << "\" }" << std::endl;
			return crow::response{os.str()};
    	}

    	else if (req.method == "DELETE"_method)
    	{
    		uint32_t retcode = server_cache.del(k);
    		if(retcode != 0)
   				{return crow::response(404);}
			return crow::response(200);
    	}

    	else
    		{return crow::response(404);}
	});

	app.port(portsaved).run();
}