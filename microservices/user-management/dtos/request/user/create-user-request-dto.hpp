#ifndef CREATE_USER_DTO_HPP
#define CREATE_USER_DTO_HPP

#include <string>

class CreateUserRequestDto {
public:
    CreateUserRequestDto() = default;
    CreateUserRequestDto(std::string fullname, std::string email)
                    : fullname(std::move(fullname)), email(std::move(email)) {}

    std::string fullname;
    std::string email;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateUserRequestDto, fullname, email);
};

#endif //CREATE_USER_DTO_HPP
