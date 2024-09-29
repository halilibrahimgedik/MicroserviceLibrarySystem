#ifndef USER_BOOK_LIST_RESPONSE_HPP
#define USER_BOOK_LIST_RESPONSE_HPP


#include <string>

class UserBookListResponse {
public:
    UserBookListResponse() = default;

    struct UserBookResponse {
        UserBookResponse() = default;
        UserBookResponse(string bookId, string name, string author, string summary, string imageUrl) :
                bookId(move(bookId)), name(move(name)), author(move(author)), summary(move(summary)),
                imageUrl(move(imageUrl)) {};

        std::string bookId;
        std::string name;
        std::string author;
        std::string summary;
        std::string imageUrl;
        std::string rentedDate;
        std::string dueDate;
        bool isDelivered {};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserBookResponse, bookId, name, author, summary, imageUrl, rentedDate, dueDate, isDelivered);
    };

    std::vector<UserBookResponse> books;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserBookListResponse, books);
};

#endif //USER_BOOK_LIST_RESPONSE_HPP
