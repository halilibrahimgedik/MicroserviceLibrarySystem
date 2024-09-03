#include <iostream>
#include <crow.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <string>
#include "../infrastructure/rabbit-mq-adapter.hpp"

using namespace std;
using json = nlohmann::json;

const string url{"amqp://guest:guest@localhost:5672/"};
const string aggregatorQueue{"aggregator"};
const string gatewayQueue{"gatewayQueue"};

std::unordered_map<string, string> requestManagerMap;

RabbitMQAdapter &setupAdapter() {
    auto &adapter = RabbitMQAdapter::getInstance();
    adapter.init(url);
    return adapter;
}

void consume(RabbitMQAdapter &adapter) {
    adapter.consume(gatewayQueue, [&adapter](const std::string_view &body, uint64_t deliveryTag, bool redelivered) {
        const std::string message(body.data(), body.size());

        if (!message.empty()) {
            json jsonMessage = json::parse(message);
            requestManagerMap[jsonMessage["requestId"].get<string>()] = message;
            adapter.ack(deliveryTag);
        }
    });

    adapter.start();
}

std::string generateRequestId() {
    const string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.*/-+";
    constexpr auto length = 16;

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<string::size_type> dist(0, characters.size() - 1);

    std::string uniqueId;
    for (auto i = 0; i < length; ++i) {
        uniqueId += characters[dist(engine)];
    }
    return uniqueId;
}

int main() {
    auto &adapter = setupAdapter();

    std::thread t([&adapter]() {
        consume(adapter);
    });

    t.detach();

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
            .methods(crow::HTTPMethod::Post)([&](const crow::request &request) {
                const auto action = request.get_header_value("action");

                if (action.empty()) {
                    return crow::response{400, "Action field cannot be empty!"};
                }

                string requestId = generateRequestId();

                json jsonData;
                if (!request.body.empty()) {
                    try {
                        jsonData = json::parse(request.body);
                    } catch (const json::exception &e) {
                        std::cout << "JSON parse error: " << e.what() << std::endl;
                        return crow::response{400, "JSON parse error"};
                    }
                }

                jsonData["action"] = action;
                jsonData["requestId"] = requestId;

                adapter.sendMessage(aggregatorQueue, jsonData.dump());

                while (requestManagerMap.empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }

                const auto iterator = requestManagerMap.find(requestId);
                if (iterator != requestManagerMap.end()) {
                    while (iterator->first != requestId) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    }
                }

                crow::response response{200, requestManagerMap[requestId]};
                response.add_header("Content-Type", "application/json");
                return response;
            });

    app.port(18080).multithreaded().run();
}