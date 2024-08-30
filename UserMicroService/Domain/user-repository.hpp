#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include <string>

#include "../Infrastructure/db-connection.hpp"
using namespace std;
namespace UserRepository {

    void inline createUser(const bsoncxx::builder::basic::document& document) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection("UsersDb","users");

        collection.insert_one(document.view());
    }

}

#endif //USER_REPOSITORY_HPP
