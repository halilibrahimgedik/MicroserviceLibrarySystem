#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP

#include "user-factory.hpp"
#include "user-repository.hpp"
#include "user.hpp"

namespace UserService {

    CreateUserResponse inline createUser(const string& fullname, const string& email, mongocxx::pool& pool) {
        const bsoncxx::builder::basic::document document = UserFactory::generateUser(fullname, email);

        return UserRepository::createUser(document, pool);
    }

    vector<User> inline getUserList(mongocxx::pool& pool) {
        return UserRepository::getUserList(pool);
    }

    User inline getUserById(const bsoncxx::oid& id, mongocxx::pool& pool) {
        return UserRepository::getUserById(id, pool);
    }

    void inline deleteUserById(const bsoncxx::oid& id, mongocxx::pool& pool) {
        UserRepository::removeUserById(id, pool);
    }

    void inline updateUser(const bsoncxx::oid& userId, const string& fullname, const string& email, const bool& isActive, mongocxx::pool& pool) {
        UserRepository::updateUser(userId, fullname, email, isActive, pool);
    }
}

#endif //USER_SERVICE_HPP
