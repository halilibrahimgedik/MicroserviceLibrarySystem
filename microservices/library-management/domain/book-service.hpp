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
        const auto document = BookFactory::generateBook(book);
        return BookRepository::createBook(document);
    }

    void inline deleteBook(const bsoncxx::oid& id) {
        BookRepository::deleteBook(id);
    }

    void inline updateBook(const Book& book) {
        BookRepository::updateBook(book);
    }

}

#endif //BOOK_SERVICE_HPP
