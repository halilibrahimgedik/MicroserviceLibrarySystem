#ifndef BOOK_REPOSITORY_HPP
#define BOOK_REPOSITORY_HPP


#include "book-factory.hpp"
#include <mongocxx/pool.hpp>

using namespace std;

namespace BookRepository {

    Book inline getBookById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        const auto result = collection.find_one(filter.view());

        return BookFactory::generateBookById(result.value());
    }

    Book inline createBook(const bsoncxx::builder::basic::document& document, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        const auto result = collection.insert_one(document.view());

        const bsoncxx::oid id = result->inserted_id().get_oid().value;

        return getBookById(id, client);
    }

    vector<Book> inline getBookList(const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        auto cursor = collection.find({});

        return BookFactory::generateBookList(cursor);
    }

    void inline deleteBook(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        collection.delete_one(filter.view());
    }

    void inline updateBook(const bsoncxx::oid& bookId, const string& name, const string& author, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", bookId));

        bsoncxx::builder::basic::document updateFilter{};
        updateFilter.append(kvp("$set",
           make_document(
                   kvp("name", name),
                   kvp("author", author)
               )
           )
       );

        collection.update_one(filter.view(), updateFilter.view());
    }

    bool inline any(const Book& book, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(
            kvp("name", book.name),
            kvp("author", book.author)
        );

        const auto result = collection.find_one(filter.view());

        return result.has_value();
    }

    Book inline addUserToBook(const bsoncxx::oid bookId, const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", bookId));

        const auto optionalValue = collection.find_one(filter.view());
        const auto book = BookFactory::generateBookById(optionalValue.value());
        const auto updateDocument = BookFactory::generateUseInfo(userId, fullname, email, rentedDate, dueDate);

        collection.update_one(filter.view(), updateDocument.view());

        return getBookById(book.id, client);;
    }

    void inline deleteUserToBooks(const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        bsoncxx::builder::basic::document update{};

        filter.append(kvp("users._id", userId));
        update.append(kvp("$pull", make_document(
            kvp("users", make_document(kvp("_id", userId)))
        )));

        collection.update_many(filter.view(), update.view());
    }

    void inline updateUserToBooks(const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, const mongocxx::pool::entry& client) {

        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("users._id", userId));

        bsoncxx::builder::basic::document update{};
        update.append(kvp("$set", make_document(
            kvp("users.$.fullname", fullname),
            kvp("users.$.email", email),
            kvp("users.$.rentedDate", bsoncxx::types::b_date(rentedDate)),
            kvp("users.$.dueDate", bsoncxx::types::b_date(dueDate))
        )));

        collection.update_many(filter.view(), update.view());
    }

}


#endif //BOOK_REPOSITORY_HPP
