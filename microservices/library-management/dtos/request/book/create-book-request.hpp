#ifndef CREATE_BOOK_REQUEST_HPP
#define CREATE_BOOK_REQUEST_HPP

#include <string>
#include "../../../../infrastructure/nlohmann-extensions.hpp"

using namespace std;
class CreateBookRequest{
public:
  CreateBookRequest() = default;
  CreateBookRequest(string bookName, string bookAuthor, string summary, string imageUrl)
    : name(std::move(bookName)), author(std::move(bookAuthor)), summary(move(summary)), imageUrl(move(imageUrl)) {};

  string name;
  string author;
  string summary;
  string imageUrl;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateBookRequest, name, author, summary, imageUrl);
};
#endif //CREATE_BOOK_REQUEST_HPP
