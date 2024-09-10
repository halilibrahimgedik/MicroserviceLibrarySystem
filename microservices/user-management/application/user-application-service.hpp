#ifndef USER_APPLICATION_SERVICE_HPP
#define USER_APPLICATION_SERVICE_HPP
#include "../domain/user-service.hpp"
#include "../domain/user.hpp"

namespace UserApplicationService {

    CreateUserResponseDto inline createUser(const CreateUserRequestDto& dto) {
        return UserService::createUser(dto);
    }

    vector<ResultUserResponseDto> inline getUserList() {
        return UserService::getUserList();
    }

    ResultUserByIdResponseDto inline getUserById(const bsoncxx::oid& id) {
        return UserService::getUserById(id);
    }

    void inline deleteUserById(const bsoncxx::oid& id) {
        UserService::deleteUserById(id);
    }

    void inline updateUser(const UpdateUserRequestDto& dto) {
        UserService::updateUser(dto);
    }

}

#endif //USER_APPLICATION_SERVICE_HPP
