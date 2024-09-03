#include <iostream>
#include <nlohmann/json.hpp>
#include "../infrastructure/rabbit-mq-adapter.hpp"
#include "configuration-helper.hpp"
#include "queue-map.hpp"

using namespace std;
using json = nlohmann::json;

void setupAdapter(RabbitMQAdapter& adapter) {
    adapter.init("amqp://guest:guest@localhost:5672/");
}

int main() {
    auto& adapter = RabbitMQAdapter::getInstance();
    setupAdapter(adapter);

    const ConfigurationHelper& configuration = ConfigurationHelper::getInstance();
    const string aggregatorQueue{configuration.getValue("aggregator")};
    const string userQueue{configuration.getValue("userQueue")};

    adapter.consume(aggregatorQueue, [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {

        const string jsonString(body.data(), body.size());
        json jsonData = json::parse(jsonString);

        if (const auto iterator{QueueMap::actionQueueMap.find(jsonData["action"].get<string>())};
            iterator != QueueMap::actionQueueMap.end())
        {
            const auto queue = iterator->second;
            adapter.sendMessage(queue, jsonString);
            adapter.ack(deliveryTag);
        } else {
            std::cerr << "Action not found in the QueueMap: " << jsonData["action"].get<string>() << endl;
        }
    });

    adapter.start();
}
