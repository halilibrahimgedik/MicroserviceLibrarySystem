#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include "application/book-application-service.hpp"
#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "../../infrastructure/utility.hpp"
#include <nlohmann/json.hpp>

#include "../../infrastructure/response-dto.hpp"
#include "dtos/request/book/create-book-request.hpp"
#include "dtos/request/user-info/user-info-request.hpp"

using json = nlohmann::json;
using namespace std;

namespace MessageListener {
    const string aggregator{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    void inline start() {
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init(url);

        adapter.consume("book.insert", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            ResponseDto message = Utility::getMessage(body.data(), body.size());

            const CreateBookRequest book { message.jsonData["name"].get<string>(), message.jsonData["author"].get<string>()};
            const auto result = BookApplicationService::createBook(book);

            message.jsonData = result;
            message.index += 1;
            message.statusCode = 201;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.getList", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            ResponseDto message = Utility::getMessage(body.data(), body.size());

            const auto bookList = BookApplicationService::getBookList();
            message.jsonData = bookList;
            message.index += 1;
            message.statusCode = 200;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.getById", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            if(ResponseDto message = Utility::getMessage(body.data(), body.size()); !message.jsonData["bookId"].get<string>().empty()) {
                const auto book = BookApplicationService::getBookById(static_cast<bsoncxx::oid>(message.jsonData["bookId"].get<string>()));

                message.jsonData = book;
                message.index += 1;
                message.statusCode = 200;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
            }
        });

        adapter.consume("book.delete", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            if(ResponseDto message = Utility::getMessage(body.data(), body.size()); !message.jsonData["id"].get<string>().empty()) {
                BookApplicationService::deleteBook(static_cast<bsoncxx::oid>(message.jsonData["id"].get<string>()));

                message.jsonData.clear();
                message.index += 1;
                message.statusCode = 204;

                adapter.sendMessage(aggregator, message.to_string());
                adapter.ack(deliveryTag);
            }
        });

        adapter.consume("book.update",[&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            ResponseDto message = Utility::getMessage(body.data(), body.size());

            const UpdateBookRequest book { message.jsonData["id"].get<string>(),message.jsonData["name"].get<string>(), message.jsonData["author"].get<string>()};

            BookApplicationService::updateBook(book);
            message.jsonData.clear();
            message.index += 1;
            message.statusCode = 204;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.addUserToBook",[&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            ResponseDto message = Utility::getMessage(body.data(), body.size());

            UserInfoRequest userInfo = message.jsonData;
            userInfo.rentedDate = chrono::system_clock::now();
            userInfo.dueDate = chrono::system_clock::now();

            const auto result = BookApplicationService::addUserToBook(
                static_cast<bsoncxx::oid>(message.jsonData["bookId"].get<string>()),
                static_cast<bsoncxx::oid>(userInfo.userId),
                userInfo.fullname,
                userInfo.email,
                userInfo.rentedDate,
                userInfo.dueDate
            );

            message.jsonData = result;
            message.index += 1;
            message.statusCode = 200;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.deleteUserToBooks",[&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            ResponseDto message = Utility::getMessage(body.data(), body.size());

            std::cerr << message.to_string();

            BookApplicationService::deleteUserToBook(static_cast<bsoncxx::oid>(message.jsonData["userId"].get<string>()));

            message.jsonData.clear();
            message.index += 1;
            message.statusCode = 204;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.updateUserToBooks",[&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            ResponseDto message = Utility::getMessage(body.data(), body.size());

            const auto userInfo = message.jsonData.get<UserInfo>();

            BookApplicationService::updateUserToBooks(static_cast<bsoncxx::oid>(message.jsonData["userId"].get<string>()), userInfo);

            message.jsonData.clear();
            message.index += 1;
            message.statusCode = 204;

            adapter.sendMessage(aggregator, message.to_string());
            adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
