#ifndef DB_CONNECTION_HPP
#define DB_CONNECTION_HPP

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

class DBConnection {
private:
    mongocxx::client client;
    DBConnection() : client{mongocxx::uri{"mongodb://localhost:2701"}} {};

public:
    static DBConnection& getInstance() {
        static DBConnection instance;
        return instance;
    }

    mongocxx::client& getClient() {
        return client;
    }

    [[nodiscard]] mongocxx::database getDatabase(const std::string& dbName) const {
        return client.database(dbName);
    }

    [[nodiscard]] mongocxx::collection getCollection(const std::string& databaseName = "BooksDb",
                                                    const std::string& collectionName = "books") const {
        const auto database = getDatabase(databaseName);
        return database[collectionName];
    }

};

#endif //DB_CONNECTION_HPP
