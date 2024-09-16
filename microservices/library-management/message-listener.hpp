#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include "application/book-application-service.hpp"
#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "../../infrastructure/utility.hpp"

#include "../../infrastructure/message-dto.hpp"
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

            message.responseData = BookApplicationService::createBook(book, client);
            message.index += 1;
            message.statusCode = 201;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.getList", [&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            const auto client = pool.acquire();

            message.responseData = BookApplicationService::getBookList(client);
            message.index += 1;
            message.statusCode = 200;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.getById", [&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            if(MessageDto message = Utility::getMessage(body.data(), body.size()); !message.serviceData["bookId"].get<string>().empty()) {
                const auto client = pool.acquire();

                message.responseData = BookApplicationService::getBookById(static_cast<bsoncxx::oid>(message.serviceData["bookId"].get<string>()), client);
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

        adapter.consume("library-management.rentBook",[&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            UserInfoRequest userInfo = message.serviceData;
            userInfo.rentedDate = chrono::system_clock::now();

            const auto client = pool.acquire();
            BookApplicationService::rentBook(
                static_cast<bsoncxx::oid>(message.serviceData["bookId"].get<string>()),
                static_cast<bsoncxx::oid>(userInfo.userId),
                userInfo.fullname,
                userInfo.email,
                userInfo.rentedDate,
                Utility::parseDate(userInfo.dueDate),
                client
            );

            message.responseData = nullptr;
            message.index += 1;
            message.statusCode = 200;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.deliverBook", [&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            message.statusCode = 400;
            if(message.serviceData.contains("bookId") && message.serviceData.contains("userId")) {
                const auto client = pool.acquire();
                const DeliverBookRequest deliverRequest { message.serviceData["bookId"].get<string>(),
                                                     message.serviceData["userId"].get<string>()};
                BookApplicationService::deliverBook(deliverRequest, client);
                message.statusCode = 204;
            }

            message.index += 1;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("library-management.getUserBookList", [&adapter, &pool](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            MessageDto message = Utility::getMessage(body.data(), body.size());

            message.statusCode = 400;
            if(message.serviceData.contains("userId")) {
                const auto client = pool.acquire();
                message.responseData  = BookApplicationService::getUserBookList(static_cast<bsoncxx::oid>(message.serviceData["userId"].get<string>()), client);
                message.statusCode = 200;
            }

            message.index += 1;

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
