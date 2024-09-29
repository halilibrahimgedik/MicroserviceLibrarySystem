#ifndef CREATE_BOOK_RESPONSE_HPP
#define CREATE_BOOK_RESPONSE_HPP

#include <string>

using namespace std;

class CreateBookResponse {
public:
    CreateBookResponse() = default;
    CreateBookResponse( string bookId, string bookName, string bookAuthor, string summary, string imageUrl)
      : bookId(move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)), summary(std::move(summary)), imageUrl(std::move(imageUrl)) {};

    string bookId;
    string name;
    string author;
    string summary;
    string imageUrl;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateBookResponse, bookId, name, author, summary, imageUrl);
};

#endif //CREATE_BOOK_RESPONSE_HPP
