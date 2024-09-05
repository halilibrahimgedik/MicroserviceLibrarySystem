#ifndef BOOK_APPLICATION_SERVICE_HPP
#define BOOK_APPLICATION_SERVICE_HPP

#include "../domain/book-service.hpp"

namespace BookApplicationService {

    Book inline getBookById(const bsoncxx::oid& id) {
        return BookService::getBookById(id);
    }

    vector<Book> inline getBookList() {
        return BookService::getBookList();
    }

    Book inline createBook(const Book& book) {
        return BookService::createBook(book);
    }

    void inline deleteBook(const bsoncxx::oid& id) {
        BookService::deleteBook(id);
    }

    void inline updateBook(const Book& book) {
        BookService::updateBook(book);
    }
}


#endif //BOOK_APPLICATION_SERVICE_HPP
