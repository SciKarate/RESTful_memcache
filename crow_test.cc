//g++ crow_test.cc -lboost_system -pthread

#include "crow.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/json")
	([]{
    crow::json::wvalue x;
    x["message"] = "Hello, World!";
    return x;
	});

	//curl -H "Content-Type: application/json" -X POST -d @user.json localhost:18080/add_json
    CROW_ROUTE(app, "/add_json")
	.methods("POST"_method, "GET"_method)
	([](const crow::request& req){
		if(req.method == "GET"_method)
		{
			if ((req.url_params.get("v") != nullptr) & (req.url_params.get("q") != nullptr))
            {
               	char x = req.url_params.get("v");
            }
            return response(200, "You used GET");
		}
		else if (req.method == "POST_method")
		{
	    	auto x = crow::json::load(req.body);
	    	if (!x)
	    	    return crow::response(400);
		    int sum = x["a"].i()+x["b"].i();
		    std::ostringstream os;
	    	os << sum;
		    os << std::endl;
		    return crow::response{os.str()};
		}
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

    app.port(18080).run();
}