#ifndef UPDATE_USER_REQUEST_DTO_HPP
#define UPDATE_USER_REQUEST_DTO_HPP


#include <string>
#include "../../../../../infrastructure/nlohmann-extensions.hpp"

class UpdateUserRequest {
public:
    UpdateUserRequest() = default;

    std::string userId;
    std::string fullname;
    std::string email;
    bool isActive {};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UpdateUserRequest, userId, fullname, email, isActive)
};

#endif //UPDATE_USER_REQUEST_DTO_HPP
