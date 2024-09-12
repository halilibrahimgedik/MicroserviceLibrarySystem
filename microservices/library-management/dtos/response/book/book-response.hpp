#ifndef BOOK_RESPONSE_HPP
#define BOOK_RESPONSE_HPP

#include <vector>

using namespace std;

class BookResponse {
public:
    BookResponse() = default;
    BookResponse( string bookId, string bookName, string bookAuthor, const vector<UserInfoResponse>& users = {})
     : bookId(move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)), users(users) {};

    string bookId;
    string name;
    string author;
    vector<UserInfoResponse> users;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookResponse, bookId, name, author, users);
};

#endif //BOOK_RESPONSE_HPP
