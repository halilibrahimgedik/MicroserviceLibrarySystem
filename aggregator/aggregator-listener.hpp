#ifndef AGGREGATOR_LISTENER_HPP
#define AGGREGATOR_LISTENER_HPP

#include <nlohmann/json.hpp>
#include "../infrastructure/rabbit-mq-adapter.hpp"
#include "queue-map.hpp"
#include "../infrastructure/response-dto.hpp"
#include "../infrastructure/utility.hpp"

using namespace std;
using json = nlohmann::json;

namespace AggregatorListener {

    void inline start() {
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init("amqp://guest:guest@localhost:5672/");

        adapter.consume("aggregator", [&adapter](const std::string_view &body, const uint64_t deliveryTag, bool redelivered) {

            ResponseDto message = Utility::getMessage(body.data(),body.size());

            const auto iterator{QueueMap::actionQueueMap.find(message.action)};
            if ( iterator != QueueMap::actionQueueMap.end()) {
                const auto& actionInfo = iterator->second;

                if(const int comingIndex = message.index == 0 ? 0 : message.index; message.index < actionInfo.queues.size()) {
                    message.index = comingIndex;
                    adapter.sendMessage(actionInfo.queues[comingIndex], message.to_string());
                }else {
                    adapter.sendMessage("gateway", message.to_string());
                }

                adapter.ack(deliveryTag);
            } else {
                std::cerr << "action not found in the QueueMap: " << message.action << endl;
            }
        });

        adapter.start();
    }
}

#endif //AGGREGATOR_LISTENER_HPP
