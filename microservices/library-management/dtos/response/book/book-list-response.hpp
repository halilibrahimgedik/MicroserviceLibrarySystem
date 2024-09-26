#ifndef BOOK_LIST_RESPONSE_HPP
#define BOOK_LIST_RESPONSE_HPP

#include <string>

class BookListResponse {
public:
    BookListResponse() = default;

    struct BookResponse {
        BookResponse() = default;
        BookResponse( std::string bookId, std::string bookName, std::string bookAuthor)
         : bookId(std::move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)) {};

        std::string bookId;
        std::string name;
        std::string author;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookResponse, bookId, name, author);
    };

    std::vector<BookResponse> books;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookListResponse, books);
};

#endif //BOOK_LIST_RESPONSE_HPP
