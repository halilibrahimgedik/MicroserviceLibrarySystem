#ifndef UPDATE_BOOK_REQUEST_HPP
#define UPDATE_BOOK_REQUEST_HPP

#include <string>
#include "../../../../../infrastructure/nlohmann-extensions.hpp"

using namespace std;

class UpdateBookRequest {
public:
    UpdateBookRequest() = default;
    UpdateBookRequest( string bookId, string bookName, string bookAuthor, string summary, string imageUrl)
     : bookId(move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)), summary(std::move(summary)), imageUrl(std::move(imageUrl)){};

    string bookId;
    string name;
    string author;
    string summary;
    string imageUrl;


    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UpdateBookRequest, bookId, name, author, summary, imageUrl);
};

#endif //UPDATE_BOOK_REQUEST_HPP
