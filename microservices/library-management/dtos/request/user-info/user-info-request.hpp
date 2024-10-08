#ifndef USER_INFO_REQUEST_HPP
#define USER_INFO_REQUEST_HPP

#include <chrono>
#include <string>
#include <utility>
#include "../../../../infrastructure/nlohmann-extensions.hpp"

using namespace std;

class UserInfoRequest {
using date = chrono::system_clock::time_point;

public:
    UserInfoRequest() = default;
    UserInfoRequest(string id, string fullname, string email)
        : userId(move(id)), fullname(move(fullname)), email(move(email)) {
    }

    string userId;
    string fullname;
    string email;
    string dueDate;
    date rentedDate;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserInfoRequest, userId, fullname, email);
};

#endif //USER_INFO_REQUEST_HPP
