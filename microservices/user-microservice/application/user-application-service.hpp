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

}

#endif //USER_APPLICATION_SERVICE_HPP
