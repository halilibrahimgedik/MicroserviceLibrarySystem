#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP

#include "user-factory.hpp"
#include "user-repository.hpp"
#include "user.hpp"

namespace UserService {

    optional<bsoncxx::oid> inline createUser(const string& fullname, const string& email, const mongocxx::pool::entry& client) {
        const bsoncxx::builder::basic::document document = UserFactory::generateUser(fullname, email);

        return UserRepository::createUser(document, client);
    }

    vector<User> inline getUserList(const mongocxx::pool::entry& client) {
        return UserRepository::getUserList(client);
    }

    User inline getUserById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        return UserRepository::getUserById(id, client);
    }

    void inline deleteUserById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        UserRepository::removeUserById(id, client);
    }

    void inline updateUser(const bsoncxx::oid& userId, const string& fullname, const string& email, const bool& isActive, const mongocxx::pool::entry& client) {
        UserRepository::updateUser(userId, fullname, email, isActive, client);
    }
}

#endif //USER_SERVICE_HPP
