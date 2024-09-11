#ifndef BOOK_APPLICATION_SERVICE_HPP
#define BOOK_APPLICATION_SERVICE_HPP

#include "../domain/book-service.hpp"
#include "../dtos/request/book/create-book-request.hpp"
#include "../dtos/request/book/update-book-request.hpp"
#include "../dtos/response/book/book-by-id-response.hpp"
#include "../dtos/response/book/book-response.hpp"
#include "../dtos/response/book/create-book-response.hpp"
#include "../dtos/response/user-info/user-info-response.hpp"


namespace BookApplicationService {

    BookByIdResponse inline getBookById(const bsoncxx::oid& id) {
        const auto book = BookService::getBookById(id);

        std::vector<UserInfoResponse> userInfoResponse;
        for (const auto& user : book.users) {
            userInfoResponse.emplace_back(user.userId.to_string(), user.fullname, user.email,user.rentedDate, user.dueDate);
        }

      return  {book.id.to_string(), book.name, book.author, userInfoResponse};
    }

    vector<BookResponse> inline getBookList() {
        const auto books = BookService::getBookList();

        vector<BookResponse> bookList;
        for (const auto& book : books) {
            BookResponse bookResponse {book.id.to_string(), book.name, book.author};

            vector<UserInfoResponse> users;
            for(const auto& user : book.users) {
                UserInfoResponse userInfoResponse {user.userId.to_string(), user.fullname, user.email, user.rentedDate, user.dueDate};
                users.push_back(move(userInfoResponse));
            }

            bookResponse.users = users;
            bookList.push_back(move(bookResponse));
        }

        return bookList;
    }

    CreateBookResponse inline createBook(const CreateBookRequest& dto) {
        const auto book = BookService::createBook(dto.name,dto.author);
        return {book.id.to_string(), book.name, book.author};
    }

    void inline deleteBook(const bsoncxx::oid& id) {
        BookService::deleteBook(id);
    }

    void inline updateBook(const UpdateBookRequest& book) {
        BookService::updateBook(static_cast<bsoncxx::oid>(book.bookId), book.name, book.author);
    }

    BookResponse inline addUserToBook(const bsoncxx::oid bookId, const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate) {

        const auto book = BookService::addUserToBook(bookId, userId, fullname, email, rentedDate, dueDate);

        std::vector<UserInfoResponse> userInfoResponse;
        for (const auto& user : book.users) {
            userInfoResponse.emplace_back(user.userId.to_string(), user.fullname, user.email,user.rentedDate, user.dueDate);
        }

        return {book.id.to_string(), book.name, book.author, userInfoResponse};
    }

    void inline deleteUserToBook(const bsoncxx::oid& userId) {
        BookService::deleteUserToBooks(userId);
    }

    void inline updateUserToBooks(const bsoncxx::oid& userId, const UserInfo& userInfo) {
        BookService::updateUserToBooks(userId, userInfo);
    }
}


#endif //BOOK_APPLICATION_SERVICE_HPP
