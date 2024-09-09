#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include "application/book-application-service.hpp"
#include "../../infrastructure/rabbit-mq-adapter.hpp"
#include "../../infrastructure/utility.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

namespace MessageListener {
    const string aggregator{"aggregator"};
    const string url{"amqp://guest:guest@localhost:5672/"};

    void inline start() {
        auto &adapter = RabbitMQAdapter::getInstance();
        adapter.init(url);

        adapter.consume("book.insert", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            json jsonData = Utility::getMessage(body.data(), body.size());

            const Book book { jsonData["name"].get<string>(), jsonData["author"].get<string>()};
            const auto result = BookApplicationService::createBook(book);

            jsonData["responseMessage"] = result;
            jsonData["index"] = jsonData["index"].get<size_t>() +1;

            adapter.sendMessage(aggregator, jsonData.dump());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.getList", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            json jsonData = Utility::getMessage(body.data(), body.size());

            const auto bookList = BookApplicationService::getBookList();
            jsonData["responseMessage"] = bookList;
            jsonData["index"] = jsonData["index"].get<size_t>() +1;

            adapter.sendMessage(aggregator, jsonData.dump());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.getById", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            if(json jsonData = Utility::getMessage(body.data(), body.size()); !jsonData["bookId"].get<string>().empty()) {
                const auto book = BookApplicationService::getBookById(static_cast<bsoncxx::oid>(jsonData["bookId"].get<string>()));

                jsonData["responseMessage"] = book;
                jsonData["index"] = jsonData["index"].get<size_t>() +1;

                adapter.sendMessage(aggregator, jsonData.dump());
                adapter.ack(deliveryTag);
            }
        });

        adapter.consume("book.delete", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            if(json jsonData = Utility::getMessage(body.data(), body.size()); !jsonData["id"].get<string>().empty()) {
                BookApplicationService::deleteBook(static_cast<bsoncxx::oid>(jsonData["id"].get<string>()));

                jsonData["index"] = jsonData["index"].get<size_t>() +1;
                jsonData["responseMessage"] = "book successfully deleted";

                adapter.sendMessage(aggregator, jsonData.dump());
                adapter.ack(deliveryTag);
            }
        });

        adapter.consume("book.update",[&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            json jsonData = Utility::getMessage(body.data(), body.size());

            const Book book { static_cast<bsoncxx::oid>(jsonData["id"].get<string>()),jsonData["name"].get<string>(),
                jsonData["author"].get<string>()};

            BookApplicationService::updateBook(book);
            jsonData["responseMessage"] = "book successfully updated";
            jsonData["index"] = jsonData["index"].get<size_t>() +1;

            adapter.sendMessage(aggregator, jsonData.dump());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.addAnUserToBook",[&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            json jsonData = Utility::getMessage(body.data(), body.size());

            UserInfo userInfo = jsonData["responseMessage"];
            userInfo.rentedDate = chrono::system_clock::now();
            userInfo.dueDate = chrono::system_clock::now();

            const auto result = BookApplicationService::addUserToBook(static_cast<bsoncxx::oid>
                                                                    (jsonData["bookId"].get<string>()), userInfo);

            jsonData["responseMessage"] = result;
            jsonData["index"] = jsonData["index"].get<size_t>() + 1;

            adapter.sendMessage(aggregator, jsonData.dump());
            adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
