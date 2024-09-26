#ifndef BOOK_BY_ID_RESPONSE_HPP
#define BOOK_BY_ID_RESPONSE_HPP

class BookByIdResponse {
public:
    BookByIdResponse() = default;
    BookByIdResponse(std::string bookId, std::string bookName, std::string bookAuthor)
         : bookId(std::move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)) {};

    std::string bookId;
    std::string name;
    std::string author;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookByIdResponse, bookId, name, author);
};

#endif //BOOK_BY_ID_RESPONSE_HPP
