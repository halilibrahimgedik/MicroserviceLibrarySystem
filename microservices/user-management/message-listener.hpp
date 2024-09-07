#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP


#include <string>
#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "application/user-application-service.hpp"
#include <nlohmann/json.hpp>

#include "../../infrastructure/utility.hpp"

using namespace std;
using json = nlohmann::json;

namespace MessageListener {
    const string aggregatorResponseQueue{"aggregator_response"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    void inline start() {
        auto& adapter = RabbitMQAdapter::getInstance();
        adapter.init(url);

        adapter.consume("user.insert",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag,bool redelivered) {
                const json jsonData = Utility::getMessage(body.data(),body.size());

                const User user{jsonData["fullname"].get<string>(), jsonData["email"].get<string>()};
                auto newUser = UserApplicationService::createUser(user);

                json responseJson;
                responseJson["next"] = "true";
                responseJson["requestId"] = jsonData["requestId"];

                responseJson["data"] = newUser;
                // we need to add this by manually.Because, the type of id is bsoncxx:oid and macro cant parse to json that
                responseJson["data"]["id"] = newUser.id.to_string();

                adapter.sendMessage(aggregatorResponseQueue, responseJson.dump());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user.getList",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                const json jsonData = Utility::getMessage(body.data(),body.size());

                const auto users = UserApplicationService::getUserList();
                json responseJson;
                responseJson["action"] = "result";
                json jsonArray = json::array();

                for (const auto &user: users) {
                    json userJson = user;
                    userJson["id"] = user.id.to_string(); // we need to add this by manually
                    jsonArray.push_back(userJson);
                }

                responseJson["data"] = jsonArray;
                responseJson["requestId"] = jsonData["requestId"];

                adapter.sendMessage(aggregatorResponseQueue, responseJson.dump());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user.getById",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                const json jsonData = Utility::getMessage(body.data(),body.size());

                if(jsonData.contains("userId")) {
                    const User user =UserApplicationService::getUserById(static_cast<bsoncxx::oid>
                                                                        (jsonData["userId"].get<string>()));
                    json responseJson;
                    responseJson["next"] = "true";
                    // responseJson["requestId"] = jsonData["requestId"];
                    responseJson["data"] = user;
                    std::cerr << responseJson.dump(4);
                    adapter.sendMessage(aggregatorResponseQueue, responseJson.dump());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user.delete",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                const json jsonData = Utility::getMessage(body.data(),body.size());

                if(!jsonData["id"].get<string>().empty()) {
                    const auto userId = static_cast<bsoncxx::oid>(jsonData["id"].get<string>());
                    // We assume we have user with same id in our db
                    UserApplicationService::deleteUserById(userId);

                    json responseJson;
                    responseJson["action"] = "result";
                    responseJson["requestId"] = jsonData["requestId"];
                    responseJson["data"] = "user deleted successfully";

                    adapter.sendMessage(aggregatorResponseQueue, responseJson.dump());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user.update",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                auto jsonData = Utility::getMessage(body.data(),body.size());

                const auto userId = static_cast<bsoncxx::oid>(jsonData["id"].get<string>());

                auto user = jsonData.get<User>();
                user.id = userId;

                UserApplicationService::updateUser(user);

                json responseJson;
                responseJson["action"] = "result";
                responseJson["requestId"] = jsonData["requestId"];
                responseJson["data"] = "user updated successfully";

                adapter.sendMessage(aggregatorResponseQueue, responseJson.dump());
                adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
