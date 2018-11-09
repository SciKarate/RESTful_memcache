//g++ crow_test.cc -lboost_system -pthread
//curl -H "Content-Type: application/json" -X POST -d @user.json localhost:18080/add_json
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

    app.port(18080).run();
}