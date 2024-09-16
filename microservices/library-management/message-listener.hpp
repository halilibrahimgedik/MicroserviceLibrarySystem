#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include "application/book-application-service.hpp"
#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "../../infrastructure/utility.hpp"
#include <nlohmann/json.hpp>

#include "../../infrastructure/message-dto.hpp"
#include "dtos/request/book/create-book-request.hpp"
#include "dtos/request/user-info/user-info-request.hpp"
#include "mongocxx/instance.hpp"

using json = nlohmann::json;
using namespace std;

namespace MessageListener {
    const string aggregator{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    void inline start() {
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init(url);

        mongocxx::instance instance{};
        mongocxx::pool pool{mongocxx::uri{"mongodb://localhost:2701"}};


        adapter.consume("library-management.insert", [&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            const CreateBookRequest book{message.serviceData["name"].get<string>(),message.serviceData["author"].get<string>()};
            const auto client = pool.acquire();
            const auto result = BookApplicationService::createBook(book, client);

            message.responseData = result;
            message.index += 1;
            message.statusCode = 201;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.getList", [&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            const auto client = pool.acquire();
            const auto bookList = BookApplicationService::getBookList(client);
            message.responseData = bookList;
            message.index += 1;
            message.statusCode = 200;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.getById", [&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            if(MessageDto message = Utility::getMessage(body.data(), body.size()); !message.serviceData["bookId"].get<string>().empty()) {
                const auto client = pool.acquire();
                const auto book = BookApplicationService::getBookById(static_cast<bsoncxx::oid>(message.serviceData["bookId"].get<string>()), client);

                message.responseData = book;
                message.index += 1;
                message.statusCode = 200;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
            }
        });

        adapter.consume("library-management.delete", [&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            if(MessageDto message = Utility::getMessage(body.data(), body.size()); !message.serviceData["id"].get<string>().empty()) {
                const auto client = pool.acquire();
                BookApplicationService::deleteBook(static_cast<bsoncxx::oid>(message.serviceData["id"].get<string>()), client);

                message.index += 1;
                message.statusCode = 204;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
            }
        });

        adapter.consume("library-management.update",[&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            const UpdateBookRequest book { message.serviceData["id"].get<string>(),
                                    message.serviceData["name"].get<string>(),
                                    message.serviceData["author"].get<string>()};

            const auto client = pool.acquire();
            BookApplicationService::updateBook(book, client);
            message.index += 1;
            message.statusCode = 204;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.addUserToBook",[&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            UserInfoRequest userInfo = message.serviceData;
            userInfo.rentedDate = chrono::system_clock::now();
            userInfo.dueDate = chrono::system_clock::now();

            const auto client = pool.acquire();
            const auto result = BookApplicationService::addUserToBook(
                static_cast<bsoncxx::oid>(message.serviceData["bookId"].get<string>()),
                static_cast<bsoncxx::oid>(userInfo.userId),
                userInfo.fullname,
                userInfo.email,
                userInfo.rentedDate,
                userInfo.dueDate,
                client
            );

            message.responseData = result;
            message.index += 1;
            message.statusCode = 200;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.deleteUserToBooks",[&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            const auto client = pool.acquire();
            BookApplicationService::deleteUserToBook(static_cast<bsoncxx::oid>(message.serviceData["userId"].get<string>()), client);

            message.index += 1;
            message.statusCode = 204;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.updateUserToBooks",[&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            const auto userInfo = message.serviceData.get<UserInfoRequest>();

            const auto client = pool.acquire();
            BookApplicationService::updateUserToBooks(userInfo, client);

            message.index += 1;
            message.statusCode = 204;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
