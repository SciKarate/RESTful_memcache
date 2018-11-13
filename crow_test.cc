//g++ crow_test.cc cache.cc -lboost_system -pthread

#include "crow.h"
#include "cache.hh"

using val_t = std::string;

int intcast(Cache::val_type vptr) //takes void ptr to int, returns int
{
	if(vptr != NULL)
		{return *((int*) vptr);}
	else
		{return 0;}
}

std::string strcast(Cache::val_type vptr) //takes void ptr to str, returns str
{
	if(vptr != NULL)
		{return *((std::string*) vptr);}
	else
		{return "0";}
}

std::string charcast(Cache::val_type vptr) //takes void ptr to char array, returns str
{
	if(vptr != NULL)
	{
		return (std::string)((char*) vptr);
	}
	else
		{return "0";}
}

int main()
{
    crow::SimpleApp app;
    int cache_size = 1028;
	Cache server_cache(cache_size);

    CROW_ROUTE(app, "/memsize").methods("GET"_method)
	([&server_cache](const crow::request& req)
	{
		std::ostringstream os;
		os << "{ memused: ";
    	os << server_cache.space_used();
	    os << " }";
	    os << std::endl;
		return crow::response{os.str()};
	});

	CROW_ROUTE(app, "/key/<string>/<string>").methods("PUT"_method)
	([&server_cache](const crow::request& req, std::string k, val_t v)
	{
		if (req.method == "PUT"_method)
		{
			val_t *vptr = new val_t(v);
			const auto& str = *static_cast<const val_t*>(vptr);
			uint32_t sz = server_cache.set(k, str.c_str(), str.size() + 1);
			delete vptr;
			if (sz == 0)
			{
				std::ostringstream os;
				os << "success" << std::endl;
				os << k << std::endl;
				os << v << std::endl;
				return crow::response{os.str()};
			}
			else
			{
				return crow::response{"value is larger than maxmem\n"};
			}
		}
		else
		{
			return crow::response(404);
		}
	});

	CROW_ROUTE(app, "/key/<str>")
	.methods("HEAD"_method, "GET"_method, "DELETE"_method)
	([&server_cache](const crow::request& req, std::string k)
	{
    	if (req.method == "HEAD"_method)
    	{
    		std::ostringstream os;
    		os << "hello!";
	    	os << std::endl;
			return crow::response{os.str()};
    	}
    	else if (req.method == "GET"_method)
    	{
    		uint32_t sz = 0;
    		Cache::val_type v = server_cache.get(k, sz);
    		std::ostringstream os;
    		os << "{ key: ";
    		os << k;
    		os << ", value: ";
    		if(v != NULL)
    		{
    			os << static_cast<const char*>(v);
    		}
    		else
    		{
    			os << "NULL";
    		}
    		os << " }";
	    	os << std::endl;
			return crow::response{os.str()};
    	}
    	else if (req.method == "DELETE"_method)
    	{
    		uint32_t retcode = server_cache.del(k);
    		if(retcode != 0)
   			{
   				return crow::response(404);
   			}
			return crow::response{"success"};
    	}
    	else
    	{
    		return crow::response(404);
    	}
	});

    CROW_ROUTE(app, "/json")
	([]
	{
    	crow::json::wvalue x;
    	x["message"] = "Hello, World!";
    	return x;
	});

	//curl -H "Content-Type: application/json" -X POST -d @user.json localhost:18080/add_json
    CROW_ROUTE(app, "/add_json").methods("POST"_method)
	([](const crow::request& req)
	{
		auto x = crow::json::load(req.body);
	    if (!x)
	        return crow::response(400);
		int sum = x["a"].i()+x["b"].i();
		std::ostringstream os;
    	os << sum;
	    os << std::endl;
		return crow::response{os.str()};
	});

	//curl localhost:18080/hello/30
	CROW_ROUTE(app,"/hello/<int>")
	([](int count)
	{
    	if (count > 100)
	        	return crow::response(400);
    	std::ostringstream os;
	    os << count << " bottles of beer!";
    	return crow::response(os.str());
	});

    app.port(18085).run();
}