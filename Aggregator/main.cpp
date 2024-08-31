#include <iostream>
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <boost/asio/io_service.hpp>
#include <nlohmann/json.hpp>

#include "configuration-helper.hpp"
#include "queue-map.hpp"

using namespace std;
using json = nlohmann::json;

int main() {
    const ConfigurationHelper& configuration = ConfigurationHelper::getInstance();
    const string aggregatorQueue {configuration.getValue("aggregator")};
    const string userQueue{configuration.getValue("userQueue")};
    const string url{configuration.getValue("AMQPConnectionUrl")};

    boost::asio::io_service service;
    AMQP::LibBoostAsioHandler handler(service);
    AMQP::TcpConnection connection(&handler, AMQP::Address(url));
    AMQP::TcpChannel channel(&connection);

    channel.declareQueue(aggregatorQueue);
    channel.declareQueue(userQueue);

    // note => onRecieved() metodu kuyruğa bir mesaj ulaştığında tetiklenir. Mesajın içeriğini işlemek veya
    // ona göre bir işlem yapmak için kullanılır.
    channel.consume(aggregatorQueue)
    .onReceived([&channel, userQueue](const AMQP::Message &message, const uint64_t deliveryTag,
                                      bool redelivered) {
        // Bu işlemleri yapmamızın nedeni, AMQP::Message nesnesinin body() fonksiyonunun ham veri ve boyutunu içermesi,
        // ve doğrudan veriyi std::string'e dönüştürme sırasında veri kaybı veya hatalar yaşanabilmesidir.
        // cout << message.body(); gibi (hatalı dönüşüm)

        // Mesajın boyutunu al =>  Verinin boyutunu bilmeden doğrudan std::string'e dönüştürmek, veri kaybına veya hatalı
        // karakterler görünmesine yol açabilir. Verinin uzunluğunu bilmek, bu veriyi doğru şekilde işlemek için önemlidir.
        const auto bodySize = message.bodySize();
        const auto body = message.body(); // Mesajın ham verisini al

        const string jsonString{body, bodySize};
        json jsonData = json::parse(jsonString);

        if (const auto iterator{QueueMap::actionQueueMap.find(jsonData["action"].get<string>())};
            iterator != QueueMap::actionQueueMap.end())
        {
            const auto queue = iterator->second; // bize kuyruğu verecek
            channel.publish("", queue, jsonString);
            channel.ack(deliveryTag);
        }else {
            std::cerr << "Action not found in the QueueMap: " << jsonData["action"].get<string>() << endl;
            // todo: action değeri map'de yoksa kuyruğa mesaj gönderemeyeceğiz, ekstra işlem yapılabilir.
            // RabbitMQ mesajın işlenmediği bilmeli araştır
        }
    });

    service.run();
}
