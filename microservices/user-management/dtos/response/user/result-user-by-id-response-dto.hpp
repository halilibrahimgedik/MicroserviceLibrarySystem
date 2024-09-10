#ifndef RESULT_USER_BY_ID_RESPONSE_DTO_HPP
#define RESULT_USER_BY_ID_RESPONSE_DTO_HPP

#include <string>
#include "../../../infrastructure/nlohmann-extensions.hpp"

class ResultUserByIdResponseDto {
public:
    ResultUserByIdResponseDto() = default;

    std::string userId;
    std::string fullname;
    std::string email;
    bool isActive {};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResultUserByIdResponseDto, userId,fullname,email,isActive);
};


#endif //RESULT_USER_BY_ID_RESPONSE_DTO_HPP
