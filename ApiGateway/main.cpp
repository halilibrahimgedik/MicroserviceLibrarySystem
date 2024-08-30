#include <iostream>
#include <crow.h>
#include <boost/asio/io_service.hpp>
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

using namespace std;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](const crow::request &request) {
        const auto action{request.get_header_value("action")};
        const string aggregatorQueue{"aggregator"};
        const string url{"amqp://guest:guest@localhost:5672/"};

        boost::asio::io_service service;
        AMQP::LibBoostAsioHandler handler(service);
        AMQP::Address address(url);
        AMQP::TcpConnection connection(&handler, address);
        AMQP::TcpChannel channel(&connection);

        channel.publish("", aggregatorQueue, action);
        service.run_for(std::chrono::milliseconds(1000));

        return crow::response{200, ""};
    });

    app.port(18080).multithreaded().run();
}

