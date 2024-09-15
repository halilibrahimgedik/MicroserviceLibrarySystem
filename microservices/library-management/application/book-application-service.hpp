#ifndef BOOK_APPLICATION_SERVICE_HPP
#define BOOK_APPLICATION_SERVICE_HPP

#include "../domain/book-service.hpp"
#include "../dtos/request/book/create-book-request.hpp"
#include "../dtos/request/book/update-book-request.hpp"
#include "../dtos/request/user-info/user-info-request.hpp"
#include "../dtos/response/book/book-by-id-response.hpp"
#include "../dtos/response/book/book-list-response.hpp"
#include "../dtos/response/book/create-book-response.hpp"
#include "../dtos/response/user-info/user-info-response.hpp"


namespace BookApplicationService {

    BookByIdResponse inline getBookById(const bsoncxx::oid& id, mongocxx::pool& pool) {
        const auto book = BookService::getBookById(id, pool);

        std::vector<UserInfoResponse> userInfoResponse;
        for (const auto& user : book.users) {
            userInfoResponse.emplace_back(user.userId.to_string(), user.fullname, user.email,user.rentedDate, user.dueDate);
        }

      return  {book.id.to_string(), book.name, book.author, userInfoResponse};
    }

    BookListResponse inline getBookList(mongocxx::pool& pool) {
        const auto books = BookService::getBookList(pool);

        BookListResponse bookList;
        for (const auto& book : books) {
            BookResponse bookResponse {book.id.to_string(), book.name, book.author};

            vector<UserInfoResponse> users;
            for(const auto& user : book.users) {
                UserInfoResponse userInfoResponse {user.userId.to_string(), user.fullname, user.email, user.rentedDate, user.dueDate};
                users.push_back(move(userInfoResponse));
            }

            bookResponse.users = users;
            bookList.books.push_back(move(bookResponse));
        }

        return bookList;
    }

    CreateBookResponse inline createBook(const CreateBookRequest& dto, mongocxx::pool& pool) {
        const auto book = BookService::createBook(dto.name,dto.author,pool);
        return {book.id.to_string(), book.name, book.author};
    }

    void inline deleteBook(const bsoncxx::oid& id, mongocxx::pool& pool) {
        BookService::deleteBook(id, pool);
    }

    void inline updateBook(const UpdateBookRequest& book, mongocxx::pool& pool) {
        BookService::updateBook(static_cast<bsoncxx::oid>(book.bookId), book.name, book.author, pool);
    }

    BookResponse inline addUserToBook(const bsoncxx::oid bookId, const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, mongocxx::pool& pool) {

        const auto book = BookService::addUserToBook(bookId, userId, fullname, email, rentedDate, dueDate, pool);

        std::vector<UserInfoResponse> userInfoResponse;
        for (const auto& user : book.users) {
            userInfoResponse.emplace_back(user.userId.to_string(), user.fullname, user.email,user.rentedDate, user.dueDate);
        }

        return {book.id.to_string(), book.name, book.author, userInfoResponse};
    }

    void inline deleteUserToBook(const bsoncxx::oid& userId, mongocxx::pool& pool) {
        BookService::deleteUserToBooks(userId, pool);
    }

    void inline updateUserToBooks(const UserInfoRequest& userInfo, mongocxx::pool& pool) {
        BookService::updateUserToBooks(static_cast<bsoncxx::oid>(userInfo.userId), userInfo.fullname, userInfo.email, userInfo.rentedDate, userInfo.dueDate, pool);
    }
}


// vector<BookResponse> inline getBookList(mongocxx::pool& pool) {
//     const auto books = BookService::getBookList(pool);
//
//     vector<BookResponse> bookList;
//     for (const auto& book : books) {
//         BookResponse bookResponse {book.id.to_string(), book.name, book.author};
//
//         vector<UserInfoResponse> users;
//         for(const auto& user : book.users) {
//             UserInfoResponse userInfoResponse {user.userId.to_string(), user.fullname, user.email, user.rentedDate, user.dueDate};
//             users.push_back(move(userInfoResponse));
//         }
//
//         bookResponse.users = users;
//         bookList.push_back(move(bookResponse));
//     }
//
//     return bookList;
// }
#endif //BOOK_APPLICATION_SERVICE_HPP
