#ifndef USER_APPLICATION_SERVICE_HPP
#define USER_APPLICATION_SERVICE_HPP

#include "../domain/user-service.hpp"
#include "../dtos/request/user/update-user-request.hpp"
#include "../dtos/response/user/user-list-response.hpp"
#include "../dtos/response/user/user-by-id-response.hpp"
#include "../dtos/request/user/create-user-request.hpp"

namespace UserApplicationService {

    optional<bsoncxx::oid> inline createUser(const CreateUserRequest& createUser, const mongocxx::pool::entry& client) {
        return UserService::createUser(createUser.fullname, createUser.email, client);
    }

    UserListResponse inline getUserList(const mongocxx::pool::entry& client) {
        const auto users = UserService::getUserList(client);

        UserListResponse userList;
        for(const auto& user : users) {
            UserResponse userResponse {user.userId.to_string(), user.fullname, user.email, user.isActive};
            userList.users.push_back(move(userResponse));
        }

        return userList;
    }

    UserByIdResponse inline getUserById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        const auto user = UserService::getUserById(id, client);
        return { user.userId.to_string(), user.fullname, user.email, user.isActive};
    }

    void inline deleteUserById(const bsoncxx::oid& id, const mongocxx::pool::entry& client) {
        UserService::deleteUserById(id, client);
    }

    void inline updateUser(const UpdateUserRequest& dto, const mongocxx::pool::entry& client) {
        UserService::updateUser(static_cast<bsoncxx::oid>(dto.userId), dto.fullname, dto.email, dto.isActive, client);
    }

}

#endif //USER_APPLICATION_SERVICE_HPP
