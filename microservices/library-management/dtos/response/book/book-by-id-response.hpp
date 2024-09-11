#ifndef BOOK_BY_ID_RESPONSE_HPP
#define BOOK_BY_ID_RESPONSE_HPP

#include <string>
#include <vector>
#include "../user-info/user-info-response.hpp"

using namespace std;

class BookByIdResponse {
public:
    BookByIdResponse() = default;
    BookByIdResponse( string bookId, string bookName, string bookAuthor, const vector<UserInfoResponse>& users = {})
     : bookId(move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)), users(users) {};

    string bookId;
    string name;
    string author;
    vector<UserInfoResponse> users;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookByIdResponse, bookId, name, author, users);
};


#endif //BOOK_BY_ID_RESPONSE_HPP
