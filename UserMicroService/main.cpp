#include <iostream>
#include <boost/asio/io_service.hpp>
#include "Application/user-application-service.hpp"
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
using json = nlohmann::json;
int main(){

    const string userQueue{"userQueue"};
    const std::string url{"amqp://guest:guest@localhost:5672/"};
    const string gatewayQueue {"gatewayQueue"};

    boost::asio::io_service service;
    AMQP::LibBoostAsioHandler handler(service);
    AMQP::TcpConnection connection(&handler, AMQP::Address(url));
    AMQP::TcpChannel channel(&connection);

    channel.declareQueue(userQueue);

    channel.consume(userQueue).onReceived([&channel, gatewayQueue](const AMQP::Message& message,
                                                const uint64_t deliveryTag, bool redelivered)
    {
        // user kuyruğundan mesajı alalım
        const size_t bodySize = message.bodySize();
        const char *body = message.body();
        const std::string command{body, bodySize};

        // user ekleme için
        if (json jsonData = json::parse(command); jsonData["action"] == "insertUser") {
            const User user {jsonData["fullname"].get<string>(), jsonData["email"].get<string>()};
            const auto newUser = UserApplicationService::createUser(user);

            string stringData;
            try {
                json jsonNewUser = newUser.toJson();
                jsonNewUser["action"] = "result";
                stringData = jsonNewUser.dump();
            } catch (json::exception &e) {
                cerr << e.what() << endl;
            }

            channel.publish("", gatewayQueue, stringData);
            channel.ack(deliveryTag);
        }
    });

    service.run();
}
