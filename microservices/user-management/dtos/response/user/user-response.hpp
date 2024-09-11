#ifndef USER_LIST_RESPONSE_DTO_HPP
#define USER_LIST_RESPONSE_DTO_HPP

#include <string>

class UserResponse {
public:
    UserResponse() = default;
    UserResponse(string userId, string fullname, string email, const bool& isActive)
        : userId(move(userId)), fullname(move(fullname)), email(move(email)), isActive(isActive) {};

    std::string userId;
    std::string fullname;
    std::string email;
    bool isActive {};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserResponse, userId,fullname,email,isActive)
};

#endif //USER_LIST_RESPONSE_DTO_HPP
