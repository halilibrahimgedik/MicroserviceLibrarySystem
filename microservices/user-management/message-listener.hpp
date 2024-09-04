#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP


#include <string>
#include <bsoncxx/v_noabi/bsoncxx/oid.hpp>

#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "application/user-application-service.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace MessageListener {
    const string aggregatorQueue{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    void inline start() {
        auto& adapter = RabbitMQAdapter::getInstance();
        adapter.init(url);

        adapter.consume("user.insert",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag,bool redelivered) {
                const std::string message(body.data(), body.size());
                const json jsonData = json::parse(message);

                const User user{jsonData["fullname"].get<string>(), jsonData["email"].get<string>()};
                auto newUser = UserApplicationService::createUser(user);

                json jsonNewUser;
                jsonNewUser["action"] = "result";
                jsonNewUser["requestId"] = jsonData["requestId"];

                jsonNewUser["data"] = newUser;
                // we need to add this by manually.Because, the type of id is bsoncxx:oid and macro cant parse to json that
                jsonNewUser["data"]["id"] = newUser.id.to_string();

                adapter.sendMessage(aggregatorQueue, jsonNewUser.dump());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user.getList",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                const std::string message(body.data(), body.size());
                const json jsonData = json::parse(message);

                const auto users = UserApplicationService::getUserList();
                json jsonSendUser;
                jsonSendUser["action"] = "result";
                json jsonArray = json::array();

                for (const auto &user: users) {
                    json userJson = user;
                    userJson["id"] = user.id.to_string(); // we need to add this by manually
                    jsonArray.push_back(userJson);
                }

                jsonSendUser["data"] = jsonArray;
                jsonSendUser["requestId"] = jsonData["requestId"];

                adapter.sendMessage(aggregatorQueue, jsonSendUser.dump());
                adapter.ack(deliveryTag);
        });


        adapter.consume("user.getById",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                const std::string message(body.data(), body.size());
                const json jsonData = json::parse(message);

                if(jsonData.contains("id")) {
                    const User user = UserApplicationService::getUserById(static_cast<bsoncxx::v_noabi::oid>(jsonData["id"].get<string>()));
                    json jsonUserInfo;
                    jsonUserInfo["action"] = "result";
                    jsonUserInfo["requestId"] = jsonData["requestId"];
                    jsonUserInfo["data"] = user;

                    adapter.sendMessage(aggregatorQueue, jsonUserInfo.dump());
                }
        });

        adapter.consume("user.delete",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {

        });

        adapter.consume("user.update",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {

        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
