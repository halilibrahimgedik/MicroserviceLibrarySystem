#ifndef BOOK_LIST_RESPONSE_HPP
#define BOOK_LIST_RESPONSE_HPP

#include <string>

class BookListResponse {
public:
    BookListResponse() = default;

    struct BookResponse {
        BookResponse() = default;
        BookResponse( std::string bookId, std::string bookName, std::string bookAuthor, std::string summary, std::string imageUrl  )
         : bookId(std::move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)), summary(std::move(summary)), imageUrl(std::move(imageUrl)) {};

        std::string bookId;
        std::string name;
        std::string author;
        std::string summary;
        std::string imageUrl;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookResponse, bookId, name, author, summary, imageUrl);
    };

    std::vector<BookResponse> books;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookListResponse, books);
};

#endif //BOOK_LIST_RESPONSE_HPP
