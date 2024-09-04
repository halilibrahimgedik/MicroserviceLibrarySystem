#include <iostream>
#include <nlohmann/json.hpp>
#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "application/user-application-service.hpp"
#include "../../libs/libcpp-event-hub/src/libcpp-event-hub.hpp"

using namespace std;
using json = nlohmann::json;

int main() {
    const string userQueue{"userQueue"};
    const string aggregatorQueue{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    auto& adapter = RabbitMQAdapter::getInstance();
    adapter.init(url);

    adapter.consume("user.insert", [&adapter, &aggregatorQueue](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
        const std::string message(body.data(), body.size());
        const json jsonData = json::parse(message);

        const User user{jsonData["fullname"].get<string>(), jsonData["email"].get<string>()};
        const auto newUser = UserApplicationService::createUser(user);

        json jsonNewUser;
        jsonNewUser["action"] = "result";
        jsonNewUser["data"] = newUser.toJson();
        jsonNewUser["requestId"] = jsonData["requestId"];

        adapter.sendMessage(aggregatorQueue, jsonNewUser.dump());
        adapter.ack(deliveryTag);
    });

    adapter.consume("user.getList", [&adapter, &aggregatorQueue](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {

        const std::string message(body.data(), body.size());
        const json jsonData = json::parse(message);

        const auto users = UserApplicationService::getUserList();
        json jsonSendUser;
        jsonSendUser["action"] = "result";
        json jsonArray = json::array();

        for (const auto &user: users) {
            jsonArray.push_back(user.toJson());
        }

        jsonSendUser["data"] = jsonArray;
        jsonSendUser["requestId"] = jsonData["requestId"];

        adapter.sendMessage(aggregatorQueue, jsonSendUser.dump());
        adapter.ack(deliveryTag);
    });


    adapter.consume("user.getById", [&adapter, aggregatorQueue](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {

    });

    adapter.consume("user.delete", [&adapter, aggregatorQueue](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {

    });

    adapter.consume("user.update", [&adapter, aggregatorQueue](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {

    });

    adapter.start();
}
