#include "crow_all.h"
int main()
{
    crow::SimpleApp app; //define your crow application

    std::atomic<int> request_count{0}; //define an atomic integer to keep track of the number of requests
    
    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([&request_count](){
        ++request_count; //increment the request count
        return "Hello world";
    });


    CROW_ROUTE(app, "/count")([&request_count](){
        return "Request count: " + std::to_string(request_count);
    });


    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).run();
}