//g++ crow_test.cc cache.cc -lboost_system -pthread

#include "crow.h"
#include "cache.hh"

int main()
{
    crow::SimpleApp app;
    int cache_size = 64;
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

	CROW_ROUTE(app, "/key/<str>/<str>").methods("PUT"_method)
	([&server_cache](const crow::request& req, std::string k, std::string v)
	{
		if (req.method == "PUT"_method)
		{
			uint32_t sz = 2;
			std::string* vsaver = new std::string(v);
			Cache::val_type vsto = (vsaver);
			sz = server_cache.set(k, vsto, sz);
			delete vsaver;
			if (sz == 0)
			{
				std::ostringstream os;
				os << "success";
				os << std::endl;
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
    		os << *((std::string*) v);
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