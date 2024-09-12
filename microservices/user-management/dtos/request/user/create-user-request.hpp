#ifndef CREATE_USER_DTO_HPP
#define CREATE_USER_DTO_HPP

#include <string>

class CreateUserRequest {
public:
    CreateUserRequest() = default;
    CreateUserRequest(std::string fullname, std::string email)
                    : fullname(std::move(fullname)), email(std::move(email)) {}

    std::string fullname;
    std::string email;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CreateUserRequest, fullname, email);
};

#endif //CREATE_USER_DTO_HPP
