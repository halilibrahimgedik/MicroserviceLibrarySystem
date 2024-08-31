#include <iostream>
#include <crow.h>
#include <boost/asio/io_service.hpp>
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <nlohmann/json.hpp>
#include <crow/json.h>
#include <optional>
using namespace std;
using json = nlohmann::json;

crow::response inline sendDataToAggregator(const optional<string>& data) {
    const string aggregatorQueue{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    boost::asio::io_service service;
    AMQP::LibBoostAsioHandler handler(service);
    AMQP::Address address(url);
    AMQP::TcpConnection connection(&handler, address);
    AMQP::TcpChannel channel(&connection);

    if (data.has_value()) {
        channel.publish("", aggregatorQueue, data.value());
        service.run_for(std::chrono::milliseconds(1000));

        return crow::response{200, "success"};
    }

    return crow::response{500, "unknown error occurred !!!"};
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](const crow::request &request) {
        const auto action{request.get_header_value("action")};

        if (action.empty()) {
            return crow::response{400, "action field can not be empty!"};
        }

        optional jsonString {action};
        if (!request.body.empty()) {
            try {
                json jsonData = {{"body", json::parse(request.body)}};
                jsonData["action"] = action;
                jsonString = jsonData.dump();
            } catch (const json::exception &e) {
                cout << "json parse error: " << e.what() << endl;
                return crow::response{400, "json parse error"};
            }
        }

        return sendDataToAggregator(jsonString);
    });

    app.port(18080).multithreaded().run();
}