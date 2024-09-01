#include <iostream>
#include <crow.h>
#include <boost/asio/io_service.hpp>
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <nlohmann/json.hpp>
#include <crow/json.h>
#include <optional>
#include <atomic>

using namespace std;
using json = nlohmann::json;

const string url{"amqp://guest:guest@localhost:5672/"};
const string aggregatorQueue{"aggregator"};
const string gatewayQueue{"gatewayQueue"};

nlohmann::json consume() {
    boost::asio::io_service service;
    AMQP::LibBoostAsioHandler handler(service);
    AMQP::Address address(url);
    AMQP::TcpConnection connection(&handler, address);
    AMQP::TcpChannel channel(&connection);

    channel.declareQueue(gatewayQueue);

    atomic<bool> messageReceived{false};
    string receivedMessage;

    channel.consume(gatewayQueue)
        .onReceived([&receivedMessage, &messageReceived, &channel]
            (const AMQP::Message &message, const uint64_t deliveryTag, bool redelivered) {
            const auto bodySize = message.bodySize();
            const auto body = message.body();
            receivedMessage.assign(body, bodySize);

            channel.ack(deliveryTag);
            messageReceived.store(true);
        });

    while (!messageReceived.load()) {
        service.run_for(std::chrono::milliseconds(60));
    }

    cout << "sonuç: "  << receivedMessage << endl;
    json jsonMessage = json::parse(receivedMessage);
    if(jsonMessage.contains("action")) {
        jsonMessage.erase("action");
    }
    return jsonMessage;
}

void publish(const string& data) {
    boost::asio::io_service service;
    AMQP::LibBoostAsioHandler handler(service);
    AMQP::Address address(url);
    AMQP::TcpConnection connection(&handler, address);
    AMQP::TcpChannel channel(&connection);

    channel.publish("", aggregatorQueue, data);
    service.run_for(std::chrono::milliseconds(10));
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](const crow::request &request) {
        const auto action{request.get_header_value("action")};

        if (action.empty()) {
            return crow::response{400, "action field can not be empty!"};
        }

        json jsonData;
        jsonData["action"] = action;
        if (!request.body.empty()) {
            try {
                jsonData = json::parse(request.body);
                jsonData["action"] = action;
            } catch (const json::exception &e) {
                cout << "json parse error: " << e.what() << endl;
                return crow::response{400, "json parse error"};
            }
        }

        publish(jsonData.dump());
        const auto resultJson = consume();

        crow::response response{200, resultJson.dump()};
        response.add_header("Content-Type", "application/json");
        return response;
    });

    app.port(18080).multithreaded().run();
}