#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include <string>
#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "application/user-application-service.hpp"
#include <nlohmann/json.hpp>

#include "../../infrastructure/response-dto.hpp"
#include "../../infrastructure/utility.hpp"

using namespace std;
using json = nlohmann::json;

namespace MessageListener {
    const string aggregator{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    void inline start() {
        auto& adapter = RabbitMQAdapter::getInstance();
        adapter.init(url);

        adapter.consume("user-management.insert",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag,bool redelivered) {
                ResponseDto message = Utility::getMessage(body.data(),body.size());

                if(!message.jsonData.empty()) {
                    const CreateUserRequest createUser = message.jsonData;
                    auto newUser = UserApplicationService::createUser(createUser);
                    message.jsonData = newUser;
                    message.statusCode = 201;
                }
                else {
                    message.statusCode = 400;
                }

                message.index += 1 ;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user-management.getList",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                ResponseDto message = Utility::getMessage(body.data(),body.size());

                const auto userListResponse = UserApplicationService::getUserList();

                message.jsonData = userListResponse.users;
                message.index += 1;
                message.statusCode = 200;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user-management.getById",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                if(ResponseDto message = Utility::getMessage(body.data(),body.size()); message.jsonData.contains("userId")) {
                    const auto user = UserApplicationService::getUserById(static_cast<bsoncxx::oid>
                                                                        (message.jsonData["userId"].get<string>()));

                    message.jsonData.update(user);
                    message.index += 1;
                    message.statusCode = 200;

                    adapter.sendMessage(aggregator, message.to_string());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user-management.delete",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                if(ResponseDto message = Utility::getMessage(body.data(),body.size()); !message.jsonData["userId"].get<string>().empty()) {
                    const auto userId = static_cast<bsoncxx::oid>(message.jsonData["userId"].get<string>());
                    // We assume we have user with same id in our db
                    UserApplicationService::deleteUserById(userId);

                    message.index += 1;
                    message.statusCode = 204;

                    adapter.sendMessage(aggregator, message.to_string());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user-management.update",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                ResponseDto message = Utility::getMessage(body.data(),body.size());

                auto user = message.jsonData.get<UpdateUserRequest>();
                UserApplicationService::updateUser(user);

                message.jsonData = user;
                message.index +=1;
                message.statusCode = 204;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
