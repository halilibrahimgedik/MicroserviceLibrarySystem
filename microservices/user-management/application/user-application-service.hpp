#ifndef USER_APPLICATION_SERVICE_HPP
#define USER_APPLICATION_SERVICE_HPP

#include "../domain/user-service.hpp"
#include "../dtos/request/user/update-user-request.hpp"
#include "../dtos/response/user/user-list-response.hpp"

namespace UserApplicationService {

    CreateUserResponse inline createUser(const CreateUserRequest& createUser) {
        return UserService::createUser(createUser.fullname, createUser.email);
    }

    UserListResponse inline getUserList() {
        const auto users = UserService::getUserList();

        UserListResponse userList;
        for(const auto& user : users) {
            UserResponse userResponse {user.userId.to_string(), user.fullname, user.email, user.isActive};
            userList.users.push_back(move(userResponse));
        }

        return userList;
    }

    UserByIdResponse inline getUserById(const bsoncxx::oid& id) {
        const auto user = UserService::getUserById(id);
        return { user.userId.to_string(), user.fullname, user.email, user.isActive};

    }

    void inline deleteUserById(const bsoncxx::oid& id) {
        UserService::deleteUserById(id);
    }

    void inline updateUser(const UpdateUserRequest& dto) {
        UserService::updateUser(static_cast<bsoncxx::oid>(dto.userId), dto.fullname, dto.email, dto.isActive);
    }

}

#endif //USER_APPLICATION_SERVICE_HPP
