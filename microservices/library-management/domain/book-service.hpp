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

    Book inline createBook(const Book& book) {
        if(BookRepository::any(book)) {
           throw runtime_error("Book already exists");
        }

        const auto document = BookFactory::generateBook(book);
        return BookRepository::createBook(document);
    }

    void inline deleteBook(const bsoncxx::oid& id) {
        BookRepository::deleteBook(id);
    }

    void inline updateBook(const Book& book) {
        BookRepository::updateBook(book);
    }

    bool inline addUserToBook(const bsoncxx::oid id, const UserInfo& userInfo) {
        return BookRepository::addUserToBook(id, userInfo);
    }


}

#endif //BOOK_SERVICE_HPP
