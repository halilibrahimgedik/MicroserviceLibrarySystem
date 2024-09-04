#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include <string>
#include "user-factory.hpp"
#include "../infrastructure/db-connection.hpp"

using namespace std;
namespace UserRepository {

    User inline getUserById(const bsoncxx::oid& id) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id",id));

        const auto result = collection.find_one(filter.view());

        auto user = UserFactory::generateUserById(result.value());

        return user;
    }

    User inline createUser(const bsoncxx::builder::basic::document& document) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        if(const auto result = collection.insert_one(document.view())) {
            const bsoncxx::oid id = result->inserted_id().get_oid().value;

            return getUserById(id);
        }

        throw runtime_error("Could not create user");
    }

    vector<User> inline getUserList() {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection("UsersDb","users");

        auto cursor = collection.find({});

        return UserFactory::generateUserList(cursor);
    }

    void inline removeUserById(const bsoncxx::oid& id) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        collection.delete_one(filter.view());
    }

    void inline updateUser(User& user) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append( kvp("_id", user.id));

        bsoncxx::builder::basic::document updateFilter{};
        updateFilter.append(kvp("$set", make_document(
            kvp("fullname", user.fullname),
            kvp("email", user.email)))
        );

        collection.update_one(filter.view(), updateFilter.view());
    }

}

#endif //USER_REPOSITORY_HPP
