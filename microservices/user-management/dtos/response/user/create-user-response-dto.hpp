#ifndef CREATE_USER_RESPONSE_DTO_HPP
#define CREATE_USER_RESPONSE_DTO_HPP


#include <string>
#include "../../../infrastructure/nlohmann-extensions.hpp"

class CreateUserResponseDto {
public:
    CreateUserResponseDto() = default;
    CreateUserResponseDto( string userId, std::string fullname, std::string email, const bool& isActive)
    : userId(move(userId)), fullname(std::move(fullname)), email(std::move(email)), isActive(isActive) {}

    std::string userId;
    std::string fullname;
    optional<std::string> email;
    bool isActive {};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateUserResponseDto, userId,fullname,email,isActive);
};

#endif //CREATE_USER_RESPONSE_DTO_HPP
