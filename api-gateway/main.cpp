#include <iostream>
#include <crow.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <string>
#include "../infrastructure/rabbit-mq-adapter.hpp"
#include "../infrastructure/utility.hpp"
#include "../libs/libcpp-event-hub/src/libcpp-event-hub.hpp"

using namespace lklibs;
using namespace std;
using json = nlohmann::json;

const string url{"amqp://guest:guest@localhost:5672/"};
const string aggregatorQueue{"aggregator"};
const string gatewayQueue{"gatewayQueue"};
auto& eventHub = EventHub::getInstance();

void consumeQueue(RabbitMQAdapter &adapter) {
    adapter.consume(gatewayQueue,
    [&adapter](const std::string_view &body, const uint64_t deliveryTag, bool redelivered) {
        const std::string message(body.data(), body.size());

        if (!message.empty()) {
            json jsonMessage = json::parse(message);
            eventHub.emit("messageConsumed", jsonMessage["requestId"].get<string>(), message);
            adapter.ack(deliveryTag);
        }
    });

    adapter.start();
}

string getData(const string& uniqueRequestId) {
    json resultJson;
    // event listener
    const auto resultListener =eventHub.addListener<string>("messageConsumed",
        [&resultJson, &uniqueRequestId](const string& eventName,const string& sender,const string& data){
            if (sender == uniqueRequestId) {
                resultJson = json::parse(data);
            }
    });

    while (resultJson.empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    eventHub.removeListener("messageConsumed", resultListener);

    return resultJson.dump();
}

int main() {
    auto &adapter = RabbitMQAdapter::getInstance();
    adapter.init(url);

    std::thread t([&adapter]() {
        consumeQueue(adapter);
    });
    t.detach();

    crow::SimpleApp app;

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::Post)([&](const crow::request &request) {
        const auto action = request.get_header_value("action");

        if (action.empty()) {
            return crow::response{400, "Action field cannot be empty!"};
        }

        string uniqueRequestId = Utility::generateUUID(); // setting unique id for each request.

        json jsonData;
        if (!request.body.empty()) {
            try {
                jsonData = json::parse(request.body);
            } catch (const json::exception &e) {
                return crow::response{400, "JSON parse error"};
            }
        }

        jsonData["action"] = action;
        jsonData["requestId"] = uniqueRequestId;

        adapter.sendMessage(aggregatorQueue, jsonData.dump());

        crow::response response{200, getData(uniqueRequestId)};
        response.add_header("Content-Type", "application/json");
        return response;
    });

    app.port(18080).multithreaded().run();
}
