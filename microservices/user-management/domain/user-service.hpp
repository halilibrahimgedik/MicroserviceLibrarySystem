#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP

#include "user-factory.hpp"
#include "user-repository.hpp"
#include "user.hpp"

namespace UserService {

    CreateUserResponse inline createUser(const string& fullname, const string& email) {
        const bsoncxx::builder::basic::document document = UserFactory::generateUser(fullname, email);

        return UserRepository::createUser(document);
    }

    vector<User> inline getUserList() {
        return UserRepository::getUserList();
    }

    User inline getUserById(const bsoncxx::oid& id) {
        return UserRepository::getUserById(id);
    }

    void inline deleteUserById(const bsoncxx::oid& id) {
        UserRepository::removeUserById(id);
    }

    void inline updateUser(const bsoncxx::oid& userId, const string& fullname, const string& email, const bool& isActive) {
        UserRepository::updateUser(userId, fullname, email, isActive);
    }
}

#endif //USER_SERVICE_HPP
