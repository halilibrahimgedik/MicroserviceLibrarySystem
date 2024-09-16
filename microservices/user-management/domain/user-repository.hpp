#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include <string>
#include <mongocxx/pool.hpp>

#include "user-factory.hpp"

using namespace std;
namespace UserRepository {

    User inline getUserById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        auto collection = (*client)["UsersDb"]["users"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id",id));

        const auto result = collection.find_one(filter.view());

        return UserFactory::generateUserById(result.value());
    }

    optional<bsoncxx::oid> inline createUser(const bsoncxx::builder::basic::document& document, const mongocxx::pool::entry& client) {
        auto collection = (*client)["UsersDb"]["users"];

        if (const auto result = collection.insert_one(document.view())) {
            return result.value().inserted_id().get_oid().value;
        }

        return nullopt;
    }

    vector<User> inline getUserList(const mongocxx::pool::entry& client) {
        auto collection = (*client)["UsersDb"]["users"];

        auto cursor = collection.find({});

        return UserFactory::generateUserList(cursor);
    }

    void inline removeUserById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        auto collection = (*client)["UsersDb"]["users"];

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        bsoncxx::builder::basic::document update{};
        update.append(kvp("$set", make_document(kvp("isActive", false))));

        collection.update_one(filter.view(), update.view());
    }

    void inline updateUser(const bsoncxx::oid& userId, const string& fullname, const string& email, const bool& isActive, const mongocxx::pool::entry& client) {
        auto collection = (*client)["UsersDb"]["users"];

        bsoncxx::builder::basic::document filter{};
        filter.append( kvp("_id", static_cast<bsoncxx::oid>(userId)));

        bsoncxx::builder::basic::document updateFilter{};
        updateFilter.append(kvp("$set", make_document(
            kvp("fullname", fullname),
            kvp("email", email),
            kvp("isActive", isActive)))
        );

        collection.update_one(filter.view(), updateFilter.view());
    }

}

#endif //USER_REPOSITORY_HPP
