#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include <string>
#include "user-factory.hpp"
#include "../dtos/request/user/update-user-request.hpp"
#include "../infrastructure/db-connection.hpp"
#include "../dtos/response/user/create-user-response.hpp"

using namespace std;
namespace UserRepository {

    User inline getUserById(const bsoncxx::oid& id) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id",id));

        const auto result = collection.find_one(filter.view());

        return UserFactory::generateUserById(result.value());
    }

    CreateUserResponse inline createUser(const bsoncxx::builder::basic::document& document) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        if (const auto result = collection.insert_one(document.view())) {
            return CreateUserResponse{
                result.value().inserted_id().get_oid().value.to_string(),
                std::string(document.view()["fullname"].get_string().value),
                std::string(document.view()["email"].get_string().value),
                document.view()["isActive"].get_bool().value,
            };
        }

        throw runtime_error("Could not create user");
    }

    vector<User> inline getUserList() {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        auto cursor = collection.find({});

        return UserFactory::generateUserList(cursor);
    }

    void inline removeUserById(const bsoncxx::oid& id) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

        bsoncxx::builder::basic::document filter{};
        filter.append(kvp("_id", id));

        bsoncxx::builder::basic::document update{};
        update.append(kvp("$set", make_document(kvp("isActive", false))));

        collection.update_one(filter.view(), update.view());
    }

    void inline updateUser(const bsoncxx::oid& userId, const string& fullname, const string& email, const bool& isActive) {
        const auto& instance = DBConnection::getInstance();
        auto collection = instance.getCollection();

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
