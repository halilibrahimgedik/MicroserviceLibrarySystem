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

        adapter.consume("user.insert",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag,bool redelivered) {
                ResponseDto message = Utility::getMessage(body.data(),body.size());

                if(!message.jsonData.empty()) {
                    const CreateUserRequest createUser = message.jsonData;
                    auto newUser = UserApplicationService::createUser(createUser);
                    message.jsonData = newUser;
                }

                message.index += 1 ;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user.getList",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                ResponseDto message = Utility::getMessage(body.data(),body.size());

                const auto users = UserApplicationService::getUserList();

                json jsonArray = json::array();

                for (const auto &resultUserResponseDto: users) {
                    json userJson = resultUserResponseDto;
                    jsonArray.push_back(userJson);
                }

                message.jsonData = jsonArray;
                message.index += 1;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.consume("user.getById",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                if(ResponseDto message = Utility::getMessage(body.data(),body.size()); message.jsonData.contains("userId")) {
                    const auto user = UserApplicationService::getUserById(static_cast<bsoncxx::oid>
                                                                        (message.jsonData["userId"].get<string>()));

                    message.jsonData.update(user);
                    std::cerr << endl << message.jsonData.dump(4) << std::endl;
                    message.index += 1;

                    adapter.sendMessage(aggregator, message.to_string());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user.delete",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                if(ResponseDto message = Utility::getMessage(body.data(),body.size()); !message.jsonData["userId"].get<string>().empty()) {
                    const auto userId = static_cast<bsoncxx::oid>(message.jsonData["userId"].get<string>());
                    // We assume we have user with same id in our db
                    UserApplicationService::deleteUserById(userId);

                    message.index += 1;

                    adapter.sendMessage(aggregator, message.to_string());
                    adapter.ack(deliveryTag);
                }
        });

        adapter.consume("user.update",
            [&adapter](const std::string_view& body, const uint64_t deliveryTag, bool redelivered) {
                ResponseDto message = Utility::getMessage(body.data(),body.size());

                auto user = message.jsonData.get<UpdateUserRequest>();
                UserApplicationService::updateUser(user);

                message.jsonData = user;
                message.index +=1;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
