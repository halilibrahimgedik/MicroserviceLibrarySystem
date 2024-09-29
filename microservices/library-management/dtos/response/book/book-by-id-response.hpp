#ifndef BOOK_BY_ID_RESPONSE_HPP
#define BOOK_BY_ID_RESPONSE_HPP

class BookByIdResponse {
public:
    BookByIdResponse() = default;
    BookByIdResponse(std::string bookId, std::string bookName, std::string bookAuthor, std::string bookSummary, std::string bookImageUrl)
        : bookId(std::move(bookId)), name(std::move(bookName)), author(std::move(bookAuthor)),
          summary(std::move(bookSummary)), imageUrl(std::move(bookImageUrl)) {};

    std::string bookId;
    std::string name;
    std::string author;
    std::string summary;
    std::string imageUrl;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookByIdResponse, bookId, name, author, summary, imageUrl);
};

#endif //BOOK_BY_ID_RESPONSE_HPP
