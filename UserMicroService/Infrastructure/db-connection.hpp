#ifndef DB_CONNECTION_HPP
#define DB_CONNECTION_HPP

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

class DBConnection {
private:
    mongocxx::client client;
    DBConnection() : client{mongocxx::uri{"mongodb://localhost:2700"}} {};

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

    [[nodiscard]] mongocxx::collection getCollection(const std::string& databaseName, const std::string& collectionName) const {
        const auto database = getDatabase(databaseName);
        return database[collectionName];
    }

};

#endif //DB_CONNECTION_HPP
