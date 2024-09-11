#ifndef UPDATE_BOOK_REQUEST_HPP
#define UPDATE_BOOK_REQUEST_HPP

#include <string>
#include "../../../../../infrastructure/nlohmann-extensions.hpp"

using namespace std;

class UpdateBookRequest {
public:
    UpdateBookRequest() = default;
    UpdateBookRequest( string bookId, string bookName, string bookAuthor)
     : bookId(move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)){};

    string bookId;
    string name;
    string author;


    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UpdateBookRequest, bookId, name, author);
};

#endif //UPDATE_BOOK_REQUEST_HPP
