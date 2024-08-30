#ifndef USER_APPLICATION_SERVICE_HPP
#define USER_APPLICATION_SERVICE_HPP
#include "../Domain/user-service.hpp"
#include "../Domain/user.hpp"

namespace UserApplicationService {

    void inline createUser(const User& user) {
        UserService::createUser(user);
    }

}

#endif //USER_APPLICATION_SERVICE_HPP
