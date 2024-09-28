#ifndef BOOK_REPOSITORY_HPP
#define BOOK_REPOSITORY_HPP


#include "book-factory.hpp"
#include <mongocxx/pool.hpp>

using namespace std;

namespace BookRepository {

    Book inline getBookById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        const auto result = collection.find_one(filter.view());

        return BookFactory::generateBookById(result.value());
    }

    Book inline getBookByIdWithUsers(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        const auto result = collection.find_one(filter.view());

        return BookFactory::generateBookByIdWithUsers(result.value());
    }

    Book inline createBook(const bsoncxx::builder::basic::document& document, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        const auto result = collection.insert_one(document.view());

        const bsoncxx::oid id = result->inserted_id().get_oid().value;

        return getBookById(id, client);
    }

    vector<Book> inline getBookList(const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        auto cursor = collection.find({});
        return BookFactory::generateBookList(cursor);
    }

    void inline deleteBook(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        collection.delete_one(filter.view());
    }

    void inline updateBook(const bsoncxx::oid& bookId, const string& name, const string& author, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", bookId));

        bsoncxx::builder::basic::document updateFilter{};
        updateFilter.append(kvp("$set",
           make_document(
                   kvp("name", name),
                   kvp("author", author)
               )
           )
       );

        collection.update_one(filter.view(), updateFilter.view());
    }

    bool inline any(const Book& book, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(
            kvp("name", book.name),
            kvp("author", book.author)
        );

        const auto result = collection.find_one(filter.view());

        return result.has_value();
    }


    vector<Book> inline getBooksWithUsers(const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        auto cursor = collection.find({});

        return BookFactory::generateBooksWithUsers(cursor);
    }

    void inline rentBook(const bsoncxx::oid bookId, const bsoncxx::oid& userId, const string& fullname, const string& email, const chrono::system_clock::time_point& rentedDate, const chrono::system_clock::time_point& dueDate, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", bookId));

        const auto optionalValue = collection.find_one(filter.view());
        const auto updateDocument = BookFactory::generateUserInfo(userId, fullname, email, rentedDate, dueDate);

        collection.update_one(filter.view(), updateDocument.view());
    }

    void inline deliverBook(const bsoncxx::oid& bookId, const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        // Filtre oluşturuluyor: kitabın _id'si eşleşmeli
        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", bookId));

        // Güncelleme filtresi oluşturuluyor: belirli kullanıcıyı bulup, isDelivered'ı true yapma
        bsoncxx::builder::basic::document update{};
        update.append(kvp("$set", make_document(
            kvp("users.$[elem].isDelivered", true)
        )));

        // Array filtrelerini oluşturma
        bsoncxx::builder::basic::array array_filters;
        array_filters.append(make_document(
            kvp("elem._id", userId)
        ));

        // Güncelleme opsiyonları (array_filters kullanılıyor)
        mongocxx::options::update update_options;
        update_options.array_filters(array_filters.view());

        // Belirli kitabın kullanıcı listesindeki userId'ye sahip kullanıcının isDelivered alanını true yap
        collection.update_one(filter.view(), update.view(), update_options);
    }

    vector<Book> inline getUserBookList(const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        // Aggregation pipeline oluşturuyoruz
        mongocxx::pipeline pipeline;

        // 1. Aşama: Filtre - users dizisinde _id'si userId ile eşleşen kitapları seçiyoruz
        pipeline.match(make_document(kvp("users._id", userId)));

        // 2. Aşama: Projeksiyon - Tüm kitap bilgilerini alıyoruz, fakat users dizisinde sadece eşleşen kullanıcıyı tutuyoruz
        pipeline.project(make_document(
            kvp("_id", 1),  // Kitap ID'sini dahil et
            kvp("name", 1),  // Kitap başlığını dahil et
            kvp("author", 1),  // Yazar bilgisi
            kvp("users", make_document(
                kvp("$filter", make_document(  // Sadece ilgili kullanıcıyı seçiyoruz
                    kvp("input", "$users"),
                    kvp("as", "user"),
                    kvp("cond", make_document(kvp("$eq", make_array("$$user._id", userId))))
                ))
            ))
        ));

        // Sorguyu çalıştırıyoruz
        auto cursor = collection.aggregate(pipeline);

        return BookFactory::generateUserBookList(cursor);
    }


    void inline deleteUserToBooks(const bsoncxx::oid& userId, const mongocxx::pool::entry& client) {
        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        bsoncxx::builder::basic::document update{};

        filter.append(kvp("users._id", userId));
        update.append(kvp("$pull", make_document(
            kvp("users", make_document(kvp("_id", userId)))
        )));

        collection.update_many(filter.view(), update.view());
    }

    void inline updateUserToBooks(const bsoncxx::oid& userId, const string& fullname, const string& email, const mongocxx::pool::entry& client) {

        auto collection = (*client)["BooksDb"]["books"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("users._id", userId));

        bsoncxx::builder::basic::document update{};
        update.append(kvp("$set", make_document(
            kvp("users.$.fullname", fullname),
            kvp("users.$.email", email)
        )));

        collection.update_many(filter.view(), update.view());
    }

}


#endif //BOOK_REPOSITORY_HPP
