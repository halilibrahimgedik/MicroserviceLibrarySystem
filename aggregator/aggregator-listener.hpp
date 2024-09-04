#ifndef AGGREGATOR_LISTENER_HPP
#define AGGREGATOR_LISTENER_HPP

#include <nlohmann/json.hpp>
#include "../infrastructure/rabbit-mq-adapter.hpp"
#include "queue-map.hpp"

using namespace std;
using json = nlohmann::json;

namespace AggregatorListener {

    void inline start() {
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init("amqp://guest:guest@localhost:5672/");

        adapter.consume("aggregator", [&adapter](const std::string_view &body,
                                                 const uint64_t deliveryTag, bool redelivered) {
            const string jsonString(body.data(), body.size());
            json jsonData = json::parse(jsonString);

            const auto iterator{QueueMap::actionQueueMap.find(jsonData["action"].get<string>())};
            if (iterator != QueueMap::actionQueueMap.end()) {
                const auto queueName = iterator->second;
                adapter.sendMessage(queueName, jsonString);
                adapter.ack(deliveryTag);
            }else {
                std::cerr << "action not found in the QueueMap: " << jsonData["action"].get<string>() << endl;
            }
        });

        adapter.start();
    }
}

#endif //AGGREGATOR_LISTENER_HPP
