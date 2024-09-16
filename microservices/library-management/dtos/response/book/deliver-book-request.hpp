#ifndef DELIVER_BOOK_REQUEST_HPP
#define DELIVER_BOOK_REQUEST_HPP

#include <string>

class DeliverBookRequest {
public:
    DeliverBookRequest(std::string bookId, std::string userId)
    : bookId(std::move(bookId)), userId(std::move(userId)) {};


    std::string bookId;
    std::string userId;
};

#endif //DELIVER_BOOK_REQUEST_HPP
