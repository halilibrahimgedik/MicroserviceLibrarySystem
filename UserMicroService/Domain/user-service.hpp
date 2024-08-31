#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP
#include "user-factory.hpp"
#include "user-repository.hpp"
#include "user.hpp"

namespace UserService {

    User inline createUser(const User& user) {
        // todo: Validation işlemleri yapılacak (user bir veya birden fazla alanı boş olabilir)
        const bsoncxx::builder::basic::document document = UserFactory::generateUser(user);

        return UserRepository::createUser(document);
    }
}

#endif //USER_SERVICE_HPP
