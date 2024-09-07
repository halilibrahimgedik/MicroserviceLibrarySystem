#ifndef HTTP_LISTENER_HPP
#define HTTP_LISTENER_HPP


#include <crow.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
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
            const string message(body.data(), body.size());

            if (!message.empty()) {
                json jsonMessage = json::parse(message);
                std::cerr <<endl<< "consume'de:  " << jsonMessage.dump(4) << endl;
                std::cerr << "consume'de requestId :   " << jsonMessage["requestId"].get<string>() << endl;

                eventHub.emit("messageConsumed", jsonMessage["requestId"].get<string>(),
                                jsonMessage["data"].dump());

                adapter.ack(deliveryTag);
            }
        });

        adapter.start();
    }

    string inline getData(const string& uniqueRequestId) {
        json resultJson;
        // event listener
        const auto resultListener =eventHub.addListener<string>("messageConsumed",
            [&resultJson, &uniqueRequestId](const string& eventName,const string& sender,const string& data){
                if (sender == uniqueRequestId) {
                    resultJson = json::parse(data);
                }
        });

        std::cerr << "resultJson: " << resultJson.dump();
        while (resultJson.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        eventHub.removeListener("messageConsumed", resultListener);

        return resultJson.dump();
    }

    void inline startApi(RabbitMQAdapter& adapter) {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/").methods(crow::HTTPMethod::Post)([&adapter](const crow::request &request) {
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
                    std::cerr << e.what() << std::endl;
                    return crow::response{400, "JSON parse error"};
                }
            }

            jsonData["action"] = action;
            jsonData["requestId"] = uniqueRequestId;

            adapter.sendMessage("aggregator", jsonData.dump());

            crow::response response{200, getData(uniqueRequestId)};
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
