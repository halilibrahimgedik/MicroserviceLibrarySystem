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

        bsoncxx::builder::basic::document updateFilter{};
        updateFilter.append(kvp("$set",
           make_document(
                   kvp("name", book.name),
                   kvp("author", book.author)
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

    Book inline addUserToBook(const bsoncxx::oid bookId, const UserInfo& userInfo) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", bookId));

        const auto optionalValue = collection.find_one(filter.view());
        const auto book = BookFactory::generateBookById(optionalValue.value());
        const auto updateDocument = BookFactory::generateUseInfo(userInfo);

        collection.update_one(filter.view(), updateDocument.view());

        return getBookById(book.id);;
    }

    void inline deleteUserToBooks(const bsoncxx::oid& userId) {
        const auto& dbConnection = DBConnection::getInstance();
        auto collection = dbConnection.getCollection();

        bsoncxx::builder::basic::document filter{};
        bsoncxx::builder::basic::document update{};

        filter.append(kvp("users._id", userId));
        update.append(kvp("$pull", make_document(
            kvp("users", make_document(kvp("_id", userId)))
        )));

        collection.update_many(filter.view(), update.view());
    }

    void inline updateUserToBooks(const bsoncxx::oid& userId, const UserInfo& userInfo) {
        const auto& dbConnection = DBConnection::getInstance();
        auto collection = dbConnection.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("users._id", userId));

        bsoncxx::builder::basic::document update{};
        update.append(kvp("$set", make_document(
            kvp("users.$.fullname", userInfo.fullname),
            kvp("users.$.email", userInfo.email)
        )));

        collection.update_many(filter.view(), update.view());
    }

}


#endif //BOOK_REPOSITORY_HPP
