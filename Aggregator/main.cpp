#include <iostream>
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <boost/asio/io_service.hpp>
#include <nlohmann/json.hpp>

using namespace std;

int main() {
    const string aggregatorQueue{"aggregator"};
    const string userQueue{"userQueue"};
    const std::string url{"amqp://guest:guest@localhost:5672/"};

    boost::asio::io_service service;
    AMQP::LibBoostAsioHandler handler(service);
    AMQP::TcpConnection connection(&handler, AMQP::Address(url));
    AMQP::TcpChannel channel(&connection);

    channel.declareQueue(aggregatorQueue);
    channel.declareQueue(userQueue);

    channel.consume(aggregatorQueue)
    .onReceived([&channel, userQueue](const AMQP::Message &message, const uint64_t deliveryTag, bool redelivered) {
        // --- std::cout << message.body(); ---
        // Bu işlemleri yapmamızın nedeni, AMQP::Message nesnesinin body() fonksiyonunun ham veri ve boyutunu içermesi ve doğrudan std::string'e dönüştürülmesi sırasında veri kaybı veya hatalar yaşanabilmesidir.

        // Mesajın boyutunu al =>  Verinin boyutunu bilmeden doğrudan std::string'e dönüştürmek, veri kaybına veya hatalı karakterler görünmesine yol açabilir. Verinin uzunluğunu bilmek, bu veriyi doğru şekilde işlemek için önemlidir.
        const size_t bodySize = message.bodySize();

        // Mesajın ham verisini al
        const char *body = message.body();

        const std::string command{body, bodySize};

        // user_queue ya istek atıcak
        if (nlohmann::json jsonData = nlohmann::json::parse(command); jsonData["action"] == "insertUser") {
            channel.publish("", userQueue, command);
        }

        // Mesajı onayla
        channel.ack(deliveryTag);
    });

    service.run();
}
