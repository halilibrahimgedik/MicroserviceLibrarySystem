#ifndef CREATE_BOOK_REQUEST_HPP
#define CREATE_BOOK_REQUEST_HPP

#include <string>
#include "../../../../infrastructure/nlohmann-extensions.hpp"

using namespace std;
class CreateBookRequest{
public:
  CreateBookRequest() = default;
  CreateBookRequest(string bookName, string bookAuthor)
    : name(std::move(bookName)), author(std::move(bookAuthor)) {};

  string name;
  string author;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateBookRequest, name, author);
};
#endif //CREATE_BOOK_REQUEST_HPP
