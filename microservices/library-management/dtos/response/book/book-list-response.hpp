#ifndef BOOK_LIST_RESPONSE_HPP
#define BOOK_LIST_RESPONSE_HPP


#include "book-response.hpp"
#include "../../../../infrastructure/nlohmann-extensions.hpp"

class BookListResponse {
public:
    BookListResponse() = default;

    vector<BookResponse> books;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookListResponse, books);
};

#endif //BOOK_LIST_RESPONSE_HPP
