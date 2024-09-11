#ifndef RESULT_USER_BY_ID_RESPONSE_DTO_HPP
#define RESULT_USER_BY_ID_RESPONSE_DTO_HPP

#include <string>
#include "../../../../../infrastructure/nlohmann-extensions.hpp"

class UserByIdResponse {
public:
    UserByIdResponse() = default;
    UserByIdResponse(string userId, string fullname, string email, const bool& isActive)
        : userId(move(userId)), fullname(move(fullname)), email(move(email)), isActive(isActive) {};

    std::string userId;
    std::string fullname;
    std::string email;
    bool isActive {};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserByIdResponse, userId,fullname,email,isActive);
};


#endif //RESULT_USER_BY_ID_RESPONSE_DTO_HPP
