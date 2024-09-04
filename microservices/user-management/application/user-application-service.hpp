#ifndef USER_APPLICATION_SERVICE_HPP
#define USER_APPLICATION_SERVICE_HPP
#include "../domain/user-service.hpp"
#include "../domain/user.hpp"

namespace UserApplicationService {

    User inline createUser(const User& user) {
        return UserService::createUser(user);
    }

    vector<User> inline getUserList() {
        return UserService::getUserList();
    }

    User inline getUserById(const bsoncxx::oid& id) {
        return UserService::getUserById(id);
    }

    void inline deleteUserById(const bsoncxx::oid& id) {
        UserService::deleteUserById(id);
    }

    void inline updateUser(User& user) {
        UserService::updateUser(user);
    }

}

#endif //USER_APPLICATION_SERVICE_HPP
