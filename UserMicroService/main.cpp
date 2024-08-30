#include <iostream>
#include <boost/asio/io_service.hpp>

#include "Application/user-application-service.hpp"
using namespace std;

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

#include <string>
using namespace std;
int main(){

    const string userQueue{"userQueue"};
    const std::string url{"amqp://guest:guest@localhost:5672/"};

    boost::asio::io_service service;
    AMQP::LibBoostAsioHandler handler(service);
    AMQP::TcpConnection connection(&handler, AMQP::Address(url));
    AMQP::TcpChannel channel(&connection);

    channel.declareQueue(userQueue);

    channel.consume(userQueue).onReceived([&channel](const AMQP::Message& message, const uint64_t deliveryTag, bool redelivered) {
        // user kuyruğundan mesajı alalım
        const size_t bodySize = message.bodySize();
        const char *body = message.body();
        const std::string command{body, bodySize};

        if (command == "insertUser") {
            const User user {"Halil İbrahim Gedik", "gedik@gmail.com"};
            UserApplicationService::createUser(user);

            channel.ack(deliveryTag);
        }
    });

    service.run();
}
