#ifndef AGGREGATOR_LISTENER_HPP
#define AGGREGATOR_LISTENER_HPP

#include <nlohmann/json.hpp>
#include "../infrastructure/rabbit-mq-adapter.hpp"
#include "queue-map.hpp"
#include "../infrastructure/utility.hpp"

using namespace std;
using json = nlohmann::json;

namespace AggregatorListener {

    void inline start() {
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init("amqp://guest:guest@localhost:5672/");

        adapter.consume("aggregator", [&adapter](const std::string_view &body, const uint64_t deliveryTag, bool redelivered) {
            json jsonData = Utility::getMessage(body.data(),body.size());
            const auto action = jsonData["action"].get<std::string>();

            std::cerr << jsonData.dump(4) << std::endl;

            // action değerini map'de arayalım
            if (const auto iterator{QueueMap::actionQueueMap.find(action)}; iterator != QueueMap::actionQueueMap.end()) {
                const auto& actionInfo = iterator->second;

                if (const size_t comingIndex = jsonData.contains("index") ?
                                        jsonData["index"].get<size_t>() : 0; comingIndex < actionInfo.queues.size()) {
                    jsonData["index"] = comingIndex;
                    adapter.sendMessage(actionInfo.queues[comingIndex], jsonData.dump());
                } else {
                    adapter.sendMessage("gateway", jsonData.dump());
                }

                adapter.ack(deliveryTag);
            } else {
                std::cerr << "action not found in the QueueMap: " << action << endl;
            }
        });

        adapter.start();
    }
}

#endif //AGGREGATOR_LISTENER_HPP
