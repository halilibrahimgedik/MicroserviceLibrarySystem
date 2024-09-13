#ifndef HTTP_LISTENER_HPP
#define HTTP_LISTENER_HPP


#include <crow.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

#include "../infrastructure/message-dto.hpp"
#include "../infrastructure/rabbit-mq-adapter.hpp"
#include "../infrastructure/utility.hpp"
#include "../libs/libcpp-event-hub/src/libcpp-event-hub.hpp"

using namespace lklibs;
using namespace std;
using json = nlohmann::json;

inline EventHub& eventHub = EventHub::getInstance();

namespace HttpListener {

    void inline consumeQueue(RabbitMQAdapter &adapter) {
        adapter.consume("gateway",
        [&adapter](const string_view &body, const uint64_t deliveryTag, bool redelivered) {
            const MessageDto response = Utility::getMessage(body.data(),body.size());

            eventHub.emit("messageConsumed", response.requestId, response);
            adapter.ack(deliveryTag);
        });

        adapter.start();
    }

    crow::response inline getData(const string& uniqueRequestId) {
        json resultJson;
        int statusCode {0};
        // event listener
        const auto resultListener =eventHub.addListener<MessageDto>("messageConsumed",
            [&resultJson, &uniqueRequestId, &statusCode](const string& eventName,const string& sender,const MessageDto& response){
                if (sender == uniqueRequestId) {

                    if(!response.responseData.empty()) {
                        resultJson["data"] = response.responseData;
                        resultJson["errors"];
                    }

                    if(response.errors.has_value()) {
                        resultJson["data"] = response.errors.value();
                        resultJson["errors"] = response.errors.value();
                    }

                    statusCode = response.statusCode;
                }
        });

        while (resultJson.empty() && statusCode == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        eventHub.removeListener("messageConsumed", resultListener);

        if(resultJson.empty()) {
            return crow:: response{statusCode};
        }

        return {statusCode, resultJson.dump()};
    }

    void inline startApi(RabbitMQAdapter& adapter) {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/").methods(crow::HTTPMethod::Post)([&adapter](const crow::request &request) {
            const auto action = request.get_header_value("action");

            if (action.empty()) {
                return crow::response{400, "Action field cannot be empty!"};
            }

            string uniqueRequestId = Utility::generateUUID(); // setting unique id for each request.

            MessageDto message;
            if (!request.body.empty()) {
                try {
                    message.serviceData = json::parse(request.body);
                } catch (const json::exception &e) {
                    std::cerr << e.what() << std::endl;
                    return crow::response{400, "JSON parse error"};
                }
            }

            message.action = action;
            message.requestId = uniqueRequestId;

            adapter.sendMessage("aggregator", message.to_string());

            crow::response response = getData(uniqueRequestId);
            response.add_header("Content-Type", "application/json");
            return response;
        });

        app.port(18080).multithreaded().run();
    }

    void inline start() {
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init("amqp://guest:guest@localhost:5672/");

        std::thread t([&adapter]() {
            consumeQueue(adapter);
        });
        t.detach();

        startApi(adapter);
    }


}

#endif //HTTP_LISTENER_HPP
