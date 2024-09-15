#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include <string>
#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "../../infrastructure/message-dto.hpp"
#include "../../infrastructure/utility.hpp"

#include "application/user-application-service.hpp"
#include <nlohmann/json.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

using namespace std;
using json = nlohmann::json;

namespace MessageListener {

    const string aggregator{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    void inline start() {
        auto& adapter = RabbitMQAdapter::getInstance();
        adapter.init(url);

        mongocxx::instance instance{};
        mongocxx::pool pool{mongocxx::uri{"mongodb://localhost:2700"}};

        adapter.consume("user-management.insert",
            [&adapter, &pool](const std::string_view& body, const uint64_t deliveryTag,bool redelivered) {
                MessageDto message = Utility::getMessage(body.data(),body.size());

                message.statusCode = 400;
                if(!message.serviceData.empty()) {
                    const CreateUserRequest createUser = message.serviceData;

                    auto newUser = UserApplicationService::createUser(createUser, pool);

                    message.responseData = newUser;
                    message.statusCode = 201;
                }

                message.index += 1 ;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user-management.getList",
            [&adapter, &pool](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                MessageDto message = Utility::getMessage(body.data(),body.size());

                const auto userListResponse = UserApplicationService::getUserList(pool);

                message.responseData = userListResponse.users;
                message.index += 1;
                message.statusCode = 200;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user-management.getById",
            [&adapter, &pool](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                if(MessageDto message = Utility::getMessage(body.data(),body.size()); message.serviceData.contains("userId")) {
                    const auto user = UserApplicationService::getUserById(
                        static_cast<bsoncxx::oid>(message.serviceData["userId"].get<string>()), pool);

                    message.responseData = user;
                    message.serviceData.update(user);
                    message.index += 1;
                    message.statusCode = 200;

                    adapter.sendMessage(aggregator, message.to_string());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user-management.delete",
            [&adapter, &pool](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                if(MessageDto message = Utility::getMessage(body.data(),body.size()); !message.serviceData["userId"].get<string>().empty()) {
                    const auto userId = static_cast<bsoncxx::oid>(message.serviceData["userId"].get<string>());
                    // We assume we have user with same id in our db
                    UserApplicationService::deleteUserById(userId, pool);

                    message.index += 1;

                    adapter.sendMessage(aggregator, message.to_string());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user-management.update",
            [&adapter, &pool](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                MessageDto message = Utility::getMessage(body.data(),body.size());

                auto user = message.serviceData.get<UpdateUserRequest>();
                UserApplicationService::updateUser(user, pool);

                message.serviceData.update(user);
                message.index +=1;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
