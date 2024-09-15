#ifndef USER_APPLICATION_SERVICE_HPP
#define USER_APPLICATION_SERVICE_HPP

#include "../domain/user-service.hpp"
#include "../dtos/request/user/update-user-request.hpp"
#include "../dtos/response/user/user-list-response.hpp"
#include "../dtos/response/user/user-by-id-response.hpp"
#include "../dtos/request/user/create-user-request.hpp"

namespace UserApplicationService {

    CreateUserResponse inline createUser(const CreateUserRequest& createUser, mongocxx::pool& pool) {
        return UserService::createUser(createUser.fullname, createUser.email, pool);
    }

    UserListResponse inline getUserList(mongocxx::pool& pool) {
        const auto users = UserService::getUserList(pool);

        UserListResponse userList;
        for(const auto& user : users) {
            UserResponse userResponse {user.userId.to_string(), user.fullname, user.email, user.isActive};
            userList.users.push_back(move(userResponse));
        }

        return userList;
    }

    UserByIdResponse inline getUserById(const bsoncxx::oid& id, mongocxx::pool& pool) {
        const auto user = UserService::getUserById(id, pool);
        return { user.userId.to_string(), user.fullname, user.email, user.isActive};

    }

    void inline deleteUserById(const bsoncxx::oid& id, mongocxx::pool& pool) {
        UserService::deleteUserById(id, pool);
    }

    void inline updateUser(const UpdateUserRequest& dto, mongocxx::pool& pool) {
        UserService::updateUser(static_cast<bsoncxx::oid>(dto.userId), dto.fullname, dto.email, dto.isActive, pool);
    }

}

#endif //USER_APPLICATION_SERVICE_HPP
