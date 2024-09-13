#ifndef AGGREGATOR_LISTENER_HPP
#define AGGREGATOR_LISTENER_HPP

#include <nlohmann/json.hpp>
#include "../infrastructure/rabbit-mq-adapter.hpp"
#include "queue-map.hpp"
#include "../infrastructure/message-dto.hpp"
#include "../infrastructure/utility.hpp"

using namespace std;
using json = nlohmann::json;

namespace AggregatorListener {

    void inline start() {
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init("amqp://guest:guest@localhost:5672/");

        adapter.consume("aggregator", [&adapter](const std::string_view &body, const uint64_t deliveryTag, bool redelivered) {
            const MessageDto message = Utility::getMessage(body.data(),body.size());

            if (const auto iterator {QueueMap::actionQueueMap.find(message.action)}; iterator != QueueMap::actionQueueMap.end()) {
                const auto& actionInfo = iterator->second;

                const int index = message.index;
                message.index < actionInfo.queues.size() ?
                    adapter.sendMessage(actionInfo.queues[index], message.to_string())
                    :
                    adapter.sendMessage("gateway", message.to_string());

                adapter.ack(deliveryTag);
            }
        });

        adapter.start();
    }
}

#endif //AGGREGATOR_LISTENER_HPP
