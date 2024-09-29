#ifndef BOOKS_WITH_USERS_RESPONSE_HPP
#define BOOKS_WITH_USERS_RESPONSE_HPP

#include <vector>
#include "../user-info/user-info-response.hpp"


class BooksWithUsersResponse {
public:
    BooksWithUsersResponse() = default;

    struct BookResponse {
        BookResponse() = default;
        BookResponse( string bookId, string bookName, string bookAuthor, string summary, string imageUrl,
                      const vector<UserInfoResponse>& users = {})
        :
        bookId(move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)),
        summary(move(summary)), imageUrl(move(imageUrl)), users(users) {};

        string bookId;
        string name;
        string author;
        string summary;
        string imageUrl;
        vector<UserInfoResponse> users;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookResponse, bookId, name, author, summary, imageUrl, users);
    };

    vector<BookResponse> books;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BooksWithUsersResponse, books);
};

#endif //BOOKS_WITH_USERS_RESPONSE_HPP
