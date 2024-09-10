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
    const string aggregator{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    void inline start() {
        auto& adapter = RabbitMQAdapter::getInstance();
        adapter.init(url);

        adapter.consume("user.insert",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag,bool redelivered) {
                json jsonData = Utility::getMessage(body.data(),body.size());

                const CreateUserRequestDto createUserDto = jsonData.get<CreateUserRequestDto>();
                auto newUser = UserApplicationService::createUser(createUserDto);

                jsonData["responseMessage"] = newUser;
                jsonData["index"] = jsonData["index"].get<size_t>() + 1 ;

                adapter.sendMessage(aggregator, jsonData.dump());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user.getList",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                json jsonData = Utility::getMessage(body.data(),body.size());

                const auto users = UserApplicationService::getUserList();

                json jsonArray = json::array();

                for (const auto &resultUserResponseDto: users) {
                    json userJson = resultUserResponseDto;
                    jsonArray.push_back(userJson);
                }

                jsonData["responseMessage"] = jsonArray;
                jsonData["index"] = jsonData["index"].get<size_t>() + 1 ;

                adapter.sendMessage(aggregator, jsonData.dump());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user.getById",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                if(json jsonData = Utility::getMessage(body.data(),body.size()); jsonData.contains("userId")) {
                    const auto user = UserApplicationService::getUserById(static_cast<bsoncxx::oid>
                                                                        (jsonData["userId"].get<string>()));

                    jsonData["responseMessage"] = user;
                    jsonData["index"] = jsonData["index"].get<size_t>() + 1;

                    adapter.sendMessage(aggregator, jsonData.dump());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user.delete",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                if(json jsonData = Utility::getMessage(body.data(),body.size()); !jsonData["userId"].get<string>().empty()) {
                    const auto userId = static_cast<bsoncxx::oid>(jsonData["userId"].get<string>());
                    // We assume we have user with same id in our db
                    UserApplicationService::deleteUserById(userId);

                    jsonData["responseMessage"] = "user deleted successfully";
                    jsonData["index"] = jsonData["index"].get<size_t>() + 1;

                    adapter.sendMessage(aggregator, jsonData.dump());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user.update",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                auto jsonData = Utility::getMessage(body.data(),body.size());

                auto user = jsonData.get<UpdateUserRequestDto>();
                UserApplicationService::updateUser(user);

                jsonData["responseMessage"] = user;
                jsonData["index"] = jsonData["index"].get<size_t>() + 1;

                adapter.sendMessage(aggregator, jsonData.dump());
                adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
