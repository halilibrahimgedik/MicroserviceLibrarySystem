#ifndef USER_BOOK_LIST_RESPONSE_HPP
#define USER_BOOK_LIST_RESPONSE_HPP


#include <string>

class UserBookListResponse {
public:
    UserBookListResponse() = default;

    struct UserBookResponse {
        std::string bookId;
        std::string name;
        std::string author;
        std::string rentedDate;
        std::string dueDate;
        bool isDelivered {};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserBookResponse, bookId, name, author, rentedDate, dueDate, isDelivered);
    };

    std::vector<UserBookResponse> books;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserBookListResponse, books);
};

#endif //USER_BOOK_LIST_RESPONSE_HPP
