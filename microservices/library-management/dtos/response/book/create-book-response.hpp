#ifndef CREATE_BOOK_RESPONSE_HPP
#define CREATE_BOOK_RESPONSE_HPP

#include <string>

using namespace std;

class CreateBookResponse {
public:
    CreateBookResponse() = default;
    CreateBookResponse( string bookId, string bookName, string bookAuthor)
      : bookId(move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)) {};

    string bookId;
    string name;
    string author;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateBookResponse, bookId, name, author);
};

#endif //CREATE_BOOK_RESPONSE_HPP
