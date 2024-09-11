#ifndef BOOK_FACTORY_HPP
#define BOOK_FACTORY_HPP

#include "book.hpp"

using namespace std;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

namespace BookFactory {

    bsoncxx::builder::basic::document inline generateBook(const string& name, const string& author) {
        bsoncxx::builder::basic::array userArray;
        bsoncxx::builder::basic::document doc;
        doc.append(
            kvp("name", name),
            kvp("author", author),
            kvp("users", userArray)
        );

        return doc;
    }

    Book inline generateBookById(const bsoncxx::document::value& docValue) {
        Book book;

        book.id = docValue["_id"].get_oid().value;
        book.name = docValue["name"].get_string().value;
        book.author = docValue["author"].get_string().value;

        if( docValue["users"] && !docValue["users"].get_array().value.empty()) {
            for (const auto& userData : docValue["users"].get_array().value) {
                UserInfo userInfo;
                userInfo.userId = userData["_id"].get_oid().value;
                userInfo.fullname = userData["fullname"].get_string().value;
                userInfo.email = userData["email"].get_string().value;
                userInfo.rentedDate = std::chrono::system_clock::time_point{std::chrono::milliseconds{userData["rentedDate"].get_date().value}};
                userInfo.dueDate = std::chrono::system_clock::time_point{std::chrono::milliseconds{userData["dueDate"].get_date().value}};

                book.users.push_back(userInfo);
            }
        }

        return book;
    }

    vector<Book> inline generateBookList(mongocxx::cursor& cursor) {
        vector<Book> books;

        for (const auto& document : cursor) {
            Book book;
            book.id = document["_id"].get_oid().value;
            book.name = document["name"].get_string().value;
            book.author = document["author"].get_string().value;

            for (const auto& userDoc : document["users"].get_array().value) {
                UserInfo userInfo;
                userInfo.userId = userDoc["_id"].get_oid().value;
                userInfo.fullname = userDoc["fullname"].get_string().value;
                userInfo.email = userDoc["email"].get_string().value;
                userInfo.rentedDate = chrono::system_clock::time_point{std::chrono::milliseconds{userDoc["rentedDate"].get_date().value}};
                userInfo.dueDate = chrono::system_clock::time_point{std::chrono::milliseconds{userDoc["dueDate"].get_date().value}};

                book.users.push_back(move(userInfo));
            }

            books.push_back(move(book));
        }

        return books;
    }

    bsoncxx::document::value inline generateUseInfo(const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate) {
        bsoncxx::builder::basic::document userInfoDocument{};
        userInfoDocument.append(kvp("_id", userId),
            kvp("fullname", fullname),
            kvp("email", email),
            kvp("rentedDate", bsoncxx::types::b_date{rentedDate}),
            kvp("dueDate", bsoncxx::types::b_date{dueDate})
        );

        const auto updateDocument = make_document(
            kvp("$push", make_document(
                kvp("users", userInfoDocument))
            )
        );

        return updateDocument;
    }

}
#endif //BOOK_FACTORY_HPP
