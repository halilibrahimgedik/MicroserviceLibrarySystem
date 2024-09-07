#ifndef BOOK_REPOSITORY_HPP
#define BOOK_REPOSITORY_HPP


#include "mongocxx/instance.hpp"
#include "book-factory.hpp"
#include "../infrustructure/db-connection.hpp"

using namespace std;

namespace BookRepository {

    Book inline getBookById(const bsoncxx::oid& id) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        const auto result = collection.find_one(filter.view());

        return BookFactory::generateBookById(result.value());
    }

    Book inline createBook(const bsoncxx::builder::basic::document& document) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        const auto result = collection.insert_one(document.view());

        const bsoncxx::oid id = result->inserted_id().get_oid().value;
        return getBookById(id);

    }

    vector<Book> inline getBookList() {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        auto cursor = collection.find({});

        return BookFactory::generateBookList(cursor);
    }

    void inline deleteBook(const bsoncxx::oid& id) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        collection.delete_one(filter.view());
    }

    void inline updateBook(const Book& book) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", book.id));

        // we could have used lambda expression inside updateFilter make_document
        bsoncxx::builder::basic::array usersArray;
        for (const auto& user : book.users) {
            usersArray.append(
                make_document(
                    kvp("fullname", user.fullname),
                    kvp("email", user.email),
                    kvp("rentedDate", bsoncxx::types::b_date{user.rentedDate}),
                    kvp("dueDate", bsoncxx::types::b_date{user.dueDate})
                )
            );
        }

        bsoncxx::builder::basic::document updateFilter{};
        updateFilter.append(kvp("$set",
           make_document(
                   kvp("name", book.name),
                   kvp("author", book.author),
                   kvp("users", usersArray)
               )
           )
       );

        collection.update_one(filter.view(), updateFilter.view());
    }

    bool inline any(const Book& book) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(
            kvp("name", book.name),
            kvp("author", book.author)
        );

        const auto result = collection.find_one(filter.view());

        return result.has_value();
    }

    bool inline addUserToBook(const bsoncxx::oid bookId, const UserInfo& userInfo) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", bookId));

        bsoncxx::builder::basic::document userInfoDocument{};
        userInfoDocument.append(kvp("id", userInfo.id),
            kvp("fullname", userInfo.fullname),
            kvp("email", userInfo.email));
        // kvp("rentedDate", userInfo.rentedDate)
        // kvp("dueDate", userInfo.dueDate)

        const auto updateDocument = make_document(
            kvp("$push", make_document(
                kvp("users", userInfoDocument)
            ))
        );

        auto result = collection.update_one(filter.view(), updateDocument.view());

        if (result && result->modified_count() > 0) {
            return true;
        }

        return false;
    }
}


#endif //BOOK_REPOSITORY_HPP
