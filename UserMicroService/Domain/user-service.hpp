#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP
#include "user-factory.hpp"
#include "user-repository.hpp"
#include "user.hpp"

namespace UserService {

    void inline createUser(const User& user) {
        const bsoncxx::builder::basic::document document = UserFactory::generateUser(user);
        UserRepository::createUser(document);
    }
}

#endif //USER_SERVICE_HPP
