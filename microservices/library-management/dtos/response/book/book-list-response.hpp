#ifndef BOOK_LIST_RESPONSE_HPP
#define BOOK_LIST_RESPONSE_HPP

#include <vector>

class BookListResponse {
public:
    BookListResponse() = default;

    struct BookResponse {
        BookResponse() = default;
        BookResponse( string bookId, string bookName, string bookAuthor, const vector<UserInfoResponse>& users = {})
         : bookId(move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)), users(users) {};

        string bookId;
        string name;
        string author;
        vector<UserInfoResponse> users;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookResponse, bookId, name, author, users);
    };

    vector<BookResponse> books;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookListResponse, books);
};

#endif //BOOK_LIST_RESPONSE_HPP
