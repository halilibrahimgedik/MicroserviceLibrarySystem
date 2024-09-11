#ifndef BOOK_SERVICE_HPP
#define BOOK_SERVICE_HPP

#include "book-repository.hpp"

namespace BookService {

    Book inline getBookById(const bsoncxx::oid& id) {
        return BookRepository::getBookById(id);
    }

    vector<Book> inline getBookList() {
        return BookRepository::getBookList();
    }

    Book inline createBook(const string& name, const string& author) {
        const auto document = BookFactory::generateBook(name, author);
        return BookRepository::createBook(document);
    }

    void inline deleteBook(const bsoncxx::oid& id) {
        BookRepository::deleteBook(id);
    }

    void inline updateBook(const bsoncxx::oid& bookId, const string& name, const string& author) {
        BookRepository::updateBook(bookId, name, author);
    }

    Book inline addUserToBook(const bsoncxx::oid bookId, const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate) {
        return BookRepository::addUserToBook(bookId, userId, fullname, email, rentedDate, dueDate);
    }

    void inline deleteUserToBooks(const bsoncxx::oid& userId) {
        BookRepository::deleteUserToBooks(userId);
    }

    void inline updateUserToBooks(const bsoncxx::oid& userId, const UserInfo& userInfo) {
        BookRepository::updateUserToBooks(userId, userInfo);
    }

}

#endif //BOOK_SERVICE_HPP
