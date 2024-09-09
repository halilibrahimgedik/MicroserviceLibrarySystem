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
            const json jsonData = Utility::getMessage(body.data(), body.size());

            vector<UserInfo> users;
            for (const auto& userJson : jsonData["users"]) {
               UserInfo userInfo = userJson;
               users.push_back(move(userInfo));
            }

            const Book book { jsonData["name"].get<string>(), jsonData["author"].get<string>(), users };
            const auto result = BookApplicationService::createBook(book);

            json resultJson;
            resultJson["next"] = "false";
            resultJson["requestId"] = jsonData["requestId"].get<string>();
            resultJson["data"] = result;

            adapter.sendMessage(aggregator, resultJson.dump());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.getList", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            const json jsonData = Utility::getMessage(body.data(), body.size());

            const auto bookList = BookApplicationService::getBookList();

            json resultJson;
            resultJson["action"] = "result";
            resultJson["requestId"] = jsonData["requestId"].get<string>();
            resultJson["data"] = bookList;

            adapter.sendMessage(aggregator, resultJson.dump());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.getById", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            const json jsonData = Utility::getMessage(body.data(), body.size());

            if(!jsonData["id"].get<string>().empty()) {
                const auto book = BookApplicationService::getBookById(static_cast<bsoncxx::oid>(jsonData["id"].get<string>()));

                json resultJson;
                resultJson["action"] = "result";
                resultJson["requestId"] = jsonData["requestId"].get<string>();
                resultJson["data"] = book;

                adapter.sendMessage(aggregator, resultJson.dump());
                adapter.ack(deliveryTag);
            } else {

            }
        });

        adapter.consume("book.delete", [&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            const json jsonData = Utility::getMessage(body.data(), body.size());

            if(!jsonData["id"].get<string>().empty()) {
                BookApplicationService::deleteBook(static_cast<bsoncxx::oid>(jsonData["id"].get<string>()));

                json resultJson;
                resultJson["action"] = "result";
                resultJson["requestId"] = jsonData["requestId"].get<string>();
                resultJson["data"] = "book (" + jsonData["id"].get<string>() + ") successfully deleted";

                adapter.sendMessage(aggregator, resultJson.dump());
                adapter.ack(deliveryTag);
            } else {
                // id alanı yoksa
            }
        });

        adapter.consume("book.update",[&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            const json jsonData = Utility::getMessage(body.data(), body.size());

            vector<UserInfo> users;
            for(const auto& userInfoJson : jsonData["users"]) {
                UserInfo userInfo = userInfoJson;
                users.push_back(std::move(userInfo));
            }

            const Book book {
                static_cast<bsoncxx::oid>(jsonData["id"].get<string>()),
                jsonData["name"].get<string>(),
                jsonData["author"].get<string>(),
                users
            };

            BookApplicationService::updateBook(book);

            json resultJson;
            resultJson["action"] = "result";
            resultJson["requestId"] = jsonData["requestId"].get<string>();
            resultJson["data"] = "book (" + jsonData["id"].get<string>() + ") successfully updated";

            adapter.sendMessage(aggregator, resultJson.dump());
            adapter.ack(deliveryTag);
        });

        adapter.consume("book.addAnUserToBook",[&adapter](const std::string_view &body, const uint64_t deliveryTag, const bool redelivered) {
            json jsonData = Utility::getMessage(body.data(), body.size());

            UserInfo userInfo = jsonData["user"];
            userInfo.rentedDate = chrono::system_clock::now();
            userInfo.dueDate = chrono::system_clock::now();

            BookApplicationService::addUserToBook(static_cast<bsoncxx::oid>(jsonData["bookId"].get<string>()), userInfo);

            jsonData["responseMessage"] = "user added to book";
            jsonData["index"] = jsonData["index"].get<size_t>() + 1;

            adapter.sendMessage(aggregator, jsonData.dump());
            adapter.ack(deliveryTag);
        });

        adapter.start();
    }
}

#endif //MESSAGE_LISTENER_HPP
