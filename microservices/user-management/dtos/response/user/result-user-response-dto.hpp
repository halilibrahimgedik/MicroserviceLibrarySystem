#ifndef USER_LIST_RESPONSE_DTO_HPP
#define USER_LIST_RESPONSE_DTO_HPP

#include <string>

class ResultUserResponseDto {
public:
    ResultUserResponseDto() = default;

    std::string userId;
    std::string fullname;
    std::string email;
    bool isActive {};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResultUserResponseDto, userId,fullname,email,isActive)
};

#endif //USER_LIST_RESPONSE_DTO_HPP
