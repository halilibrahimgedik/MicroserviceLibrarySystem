#ifndef BOOK_FACTORY_HPP
#define BOOK_FACTORY_HPP

#include "book.hpp"

using namespace std;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

namespace BookFactory {

    bsoncxx::builder::basic::document inline generateBook(const Book& book) {
        bsoncxx::builder::basic::document doc;

        doc.append(
            kvp("name", book.name),
            kvp("author", book.author)
        );

        bsoncxx::builder::basic::array users;
        for (const auto& user : book.users) {
            bsoncxx::builder::basic::document userDoc;
            userDoc.append(
                kvp("_id", user.id),
                kvp("fullname", user.fullname),
                kvp("email", user.email),
                kvp("rentedDate", bsoncxx::types::b_date{user.rentedDate}),
                kvp("dueDate", bsoncxx::types::b_date{user.dueDate})
            );
            users.append(userDoc);
        }

        doc.append(kvp("users", users) );

        return doc;
    }

    Book inline generateBookById(const bsoncxx::document::value& docValue) {
        Book book;
        const auto docView = docValue.view();

        book.id = docView["_id"].get_oid().value;
        book.name = docView["name"].get_string().value;
        book.author = docView["author"].get_string().value;

        for (const auto& userData : docView["users"].get_array().value) {
            UserInfo userInfo;
            userInfo.id = userData["_id"].get_oid().value;
            userInfo.fullname = userData["fullname"].get_string().value;
            userInfo.email = userData["email"].get_string().value;
            userInfo.rentedDate = std::chrono::system_clock::time_point{std::chrono::milliseconds{
                userData["rentedDate"].get_date().value}
            };
            userInfo.dueDate = std::chrono::system_clock::time_point{std::chrono::milliseconds{
                userData["dueDate"].get_date().value}
            };

            book.users.push_back(userInfo);
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
                userInfo.id = userDoc["_id"].get_oid().value;
                userInfo.fullname = userDoc["fullname"].get_string().value;
                userInfo.email = userDoc["email"].get_string().value;
                userInfo.rentedDate = chrono::system_clock::time_point{
                    std::chrono::milliseconds{userDoc["rentedDate"].get_date().value}
                };
                userInfo.dueDate = chrono::system_clock::time_point{
                    std::chrono::milliseconds{userDoc["dueDate"].get_date().value}
                };

                book.users.push_back(move(userInfo));
            }

            books.push_back(move(book));
        }

        return books;
    }

}
#endif //BOOK_FACTORY_HPP
