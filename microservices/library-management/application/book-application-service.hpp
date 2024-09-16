#ifndef BOOK_APPLICATION_SERVICE_HPP
#define BOOK_APPLICATION_SERVICE_HPP

#include "../../../infrastructure/utility.hpp"
#include "../domain/book-service.hpp"
#include "../dtos/request/book/create-book-request.hpp"
#include "../dtos/request/book/update-book-request.hpp"
#include "../dtos/request/user-info/user-info-request.hpp"
#include "../dtos/response/book/book-by-id-response.hpp"
#include "../dtos/response/book/book-list-response.hpp"
#include "../dtos/response/book/create-book-response.hpp"
#include "../dtos/response/book/deliver-book-request.hpp"
#include "../dtos/response/book/user-book-list-response.hpp"
#include "../dtos/response/user-info/user-info-response.hpp"


namespace BookApplicationService {

    BookByIdResponse inline getBookById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        const auto book = BookService::getBookById(id, client);

        std::vector<UserInfoResponse> userInfoResponse;
        for (const auto& user : book.users) {
            userInfoResponse.emplace_back(user.userId.to_string(), user.fullname, user.email, Utility::formatDate(user.rentedDate), Utility::formatDate(user.dueDate), user.isDelivered);
        }

      return  {book.id.to_string(), book.name, book.author, userInfoResponse};
    }

    BookListResponse inline getBookList(const mongocxx::pool::entry& client) {
        const auto books = BookService::getBookList(client);

        BookListResponse bookList;
        for (const auto& book : books) {

            BookListResponse::BookResponse bookResponse {book.id.to_string(), book.name, book.author};
            for(const auto& user : book.users) {
                UserInfoResponse userInfoResponse {user.userId.to_string(), user.fullname, user.email, Utility::formatDate(user.rentedDate), Utility::formatDate(user.dueDate), user.isDelivered};
                bookResponse.users.push_back(move(userInfoResponse));
            }

            bookList.books.push_back(move(bookResponse));
        }

        return bookList;
    }

    CreateBookResponse inline createBook(const CreateBookRequest& dto, const mongocxx::pool::entry& client) {
        const auto book = BookService::createBook(dto.name,dto.author,client);
        return {book.id.to_string(), book.name, book.author};
    }

    void inline deleteBook(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        BookService::deleteBook(id, client);
    }

    void inline updateBook(const UpdateBookRequest& book, const mongocxx::pool::entry& client) {
        BookService::updateBook(static_cast<bsoncxx::oid>(book.bookId), book.name, book.author, client);
    }

    void inline rentBook(const bsoncxx::oid bookId, const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, const mongocxx::pool::entry& client) {

        BookService::rentBook(bookId, userId, fullname, email, rentedDate, dueDate, client);
    }

    void inline deliverBook(const DeliverBookRequest& request, const mongocxx::pool::entry& client) {
        BookService::deliverBook(static_cast<bsoncxx::oid>(request.bookId), static_cast<bsoncxx::oid>(request.userId), client);
    }

    UserBookListResponse inline getUserBookList(const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        const auto books = BookService::getUserBookList(userId, client);

        UserBookListResponse bookList;
        for(const auto& book : books) {
            UserBookListResponse::UserBookResponse userBookResponse{book.id.to_string(),book.name,book.author};

            for(const auto& user : book.users) {
                userBookResponse.dueDate = Utility::formatDate(user.dueDate);
                userBookResponse.rentedDate = Utility::formatDate(user.rentedDate);
                userBookResponse.isDelivered = user.isDelivered;
            }

            bookList.books.push_back(std::move(userBookResponse));
        }

        return bookList;
    }

    void inline deleteUserToBook(const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        BookService::deleteUserToBooks(userId, client);
    }

    void inline updateUserToBooks(const UserInfoRequest& userInfo, const mongocxx::pool::entry& client) {
        BookService::updateUserToBooks(static_cast<bsoncxx::oid>(userInfo.userId), userInfo.fullname, userInfo.email, userInfo.rentedDate, Utility::parseDate(userInfo.dueDate), client);
    }
}
#endif //BOOK_APPLICATION_SERVICE_HPP
