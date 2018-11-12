//g++ crow_test.cc cache.cc -lboost_system -pthread

#include "crow.h"
#include "cache.hh"

int cache_size = 64;
Cache server_cache(cache_size);

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/memsize")
	.methods("GET"_method)
	([](const crow::request& req){
		std::ostringstream os;
    	os << server_cache.space_used();
	    os << std::endl;
		return crow::response{os.str()};
	});

	CROW_ROUTE(app, "/key/<str>")
	.methods("HEAD"_method, "GET"_method, "DELETE"_method)
	([](const crow::request& req, std::string k){
    	if (req.method == "HEAD"_method)
    	{
    		std::ostringstream os;
    		os << "hello!";
	    	os << std::endl;
			return crow::response{os.str()};
    	}
    	else if (req.method == "GET"_method)
    	{
    		std::ostringstream os;
    		os << k;
	    	os << std::endl;
			return crow::response{os.str()};
    	}
    	else if (req.method == "DELETE"_method)
    	{
    		std::ostringstream os;
    		os << "howdy";
    		os << k;
	    	os << std::endl;
			return crow::response{os.str()};
    	}
    	else
    	{
    		return crow::response(404);
    	}
	});

    CROW_ROUTE(app, "/json")
	([]{
    crow::json::wvalue x;
    x["message"] = "Hello, World!";
    return x;
	});

	//curl -H "Content-Type: application/json" -X POST -d @user.json localhost:18080/add_json
    CROW_ROUTE(app, "/add_json")
	.methods("POST"_method)
	([](const crow::request& req){
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
	([](int count){
    	if (count > 100)
	        	return crow::response(400);
    	std::ostringstream os;
	    os << count << " bottles of beer!";
    	return crow::response(os.str());
	});

    app.port(18085).run();
}