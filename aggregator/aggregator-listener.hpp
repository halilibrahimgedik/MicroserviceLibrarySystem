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
        bool gotResponse {false};
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init("amqp://guest:guest@localhost:5672/");
        json jsonData;

        adapter.consume("aggregator", [&adapter, &gotResponse, &jsonData](const std::string_view &body, const uint64_t deliveryTag, bool redelivered) {
            json jsonRequestData = Utility::getMessage(body.data(),body.size());
            const auto action = jsonRequestData["action"].get<string>();

            // action değerini map'de arayalım
            if (const auto iterator{QueueMap::actionQueueMap.find(action)}; iterator != QueueMap::actionQueueMap.end()) {
                const auto& actionInfo = iterator->second;
                adapter.ack(deliveryTag);

                if (jsonData.empty()) {
                    jsonData = jsonRequestData;
                }
                jsonData["requestId"] = jsonRequestData["requestId"].get<string>();

                // belirtilen kuyruklara sırayla mesajı yollayalım
                for (const auto & queueName : actionInfo.queues) {
                    adapter.sendMessage(queueName, jsonData.dump());
                    gotResponse = false;

                    while (!gotResponse) {
                        this_thread::sleep_for(chrono::milliseconds(5));
                    }
                }
            } else {
                std::cerr << "action not found in the QueueMap: " << action << endl;
            }
        });

        adapter.consume("aggregator_response", [&adapter, &gotResponse, &jsonData](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
            json jsonResponseData = Utility::getMessage(body.data(), body.size());
            const string nextString = jsonResponseData["next"].get<string>();
            const bool next = (nextString == "true");

            jsonData["data"] = jsonResponseData["data"];

            if(!next) {
                adapter.sendMessage("gateway", jsonData.dump() );
            }

            adapter.ack(deliveryTag);
            gotResponse = true;
        });

        adapter.start();
    }
}

#endif //AGGREGATOR_LISTENER_HPP
