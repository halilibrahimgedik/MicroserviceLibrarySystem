#ifndef BOOK_SERVICE_HPP
#define BOOK_SERVICE_HPP

#include "book-repository.hpp"

namespace BookService {

    Book inline getBookById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        return BookRepository::getBookById(id, client);
    }

    vector<Book> inline getBookList(const mongocxx::pool::entry& client) {
        return BookRepository::getBookList(client);
    }

    Book inline createBook(const string& name, const string& author, const mongocxx::pool::entry& client) {
        const auto document = BookFactory::generateBook(name, author);
        return BookRepository::createBook(document, client);
    }

    void inline deleteBook(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        BookRepository::deleteBook(id, client);
    }

    void inline updateBook(const bsoncxx::oid& bookId, const string& name, const string& author, const mongocxx::pool::entry& client) {
        BookRepository::updateBook(bookId, name, author, client);
    }

    void inline rentBook(const bsoncxx::oid bookId, const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, const mongocxx::pool::entry& client) {
        BookRepository::rentBook(bookId, userId, fullname, email, rentedDate, dueDate, client);
    }

    void inline deliverBook(const bsoncxx::oid& bookId, const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        BookRepository::deliverBook(bookId, userId, client);
    }

    vector<Book> inline getUserBookList(const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        return BookRepository::getUserBookList(userId, client);
    }

    void inline deleteUserToBooks(const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        BookRepository::deleteUserToBooks(userId, client);
    }

    void inline updateUserToBooks(const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, const mongocxx::pool::entry& client) {
        BookRepository::updateUserToBooks(userId, fullname, email, rentedDate, dueDate, client);
    }

}

#endif //BOOK_SERVICE_HPP
