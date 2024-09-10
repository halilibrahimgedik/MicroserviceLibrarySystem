#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP

#include "user-factory.hpp"
#include "user-repository.hpp"
#include "user.hpp"

namespace UserService {

    CreateUserResponseDto inline createUser(const CreateUserRequestDto& dto) {
        // todo: Validation işlemleri yapılacak (user bir veya birden fazla alanı boş olabilir)
        const bsoncxx::builder::basic::document document = UserFactory::generateUser(dto);

        return UserRepository::createUser(document);
    }

    vector<ResultUserResponseDto> inline getUserList() {
        return UserRepository::getUserList();
    }

    ResultUserByIdResponseDto inline getUserById(const bsoncxx::oid& id) {
        return UserRepository::getUserById(id);
    }

    void inline deleteUserById(const bsoncxx::oid& id) {
        UserRepository::removeUserById(id);
    }

    void inline updateUser(const UpdateUserRequestDto& dto) {
        UserRepository::updateUser(dto);
    }
}

#endif //USER_SERVICE_HPP
