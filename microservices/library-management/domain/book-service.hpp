#ifndef BOOK_SERVICE_HPP
#define BOOK_SERVICE_HPP

#include "book-repository.hpp"

namespace BookService {

    Book inline getBookById(const bsoncxx::oid& id, mongocxx::pool& pool) {
        return BookRepository::getBookById(id, pool);
    }

    vector<Book> inline getBookList(mongocxx::pool& pool) {
        return BookRepository::getBookList(pool);
    }

    Book inline createBook(const string& name, const string& author, mongocxx::pool& pool) {
        const auto document = BookFactory::generateBook(name, author);
        return BookRepository::createBook(document, pool);
    }

    void inline deleteBook(const bsoncxx::oid& id, mongocxx::pool& pool) {
        BookRepository::deleteBook(id, pool);
    }

    void inline updateBook(const bsoncxx::oid& bookId, const string& name, const string& author, mongocxx::pool& pool) {
        BookRepository::updateBook(bookId, name, author, pool);
    }

    Book inline addUserToBook(const bsoncxx::oid bookId, const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, mongocxx::pool& pool) {
        return BookRepository::addUserToBook(bookId, userId, fullname, email, rentedDate, dueDate, pool);
    }

    void inline deleteUserToBooks(const bsoncxx::oid& userId, mongocxx::pool& pool) {
        BookRepository::deleteUserToBooks(userId, pool);
    }

    void inline updateUserToBooks(const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, mongocxx::pool& pool) {
        BookRepository::updateUserToBooks(userId, fullname, email, rentedDate, dueDate, pool);
    }

}

#endif //BOOK_SERVICE_HPP
