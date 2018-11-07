//g++ pistache_server.cc -lpistache -pthread
#include <pistache/endpoint.h>

using namespace Pistache;

class HelloHandler : public Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    std::string apple = "apple";
    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
         response.send(Http::Code::Ok, apple);
    }
};

int main() {
    Address addr(Ipv4::any(), Port(9081));

    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serve();
}