#ifndef USER_INFO_RESPONSE_HPP
#define USER_INFO_RESPONSE_HPP

#include <chrono>
#include <string>

using namespace std;

class UserInfoResponse {
    using date = chrono::system_clock::time_point;
public:
    UserInfoResponse() = default;
    UserInfoResponse(string id, string fullname, string email, const date& rentedDate, const date& dueDate)
        : userId(move(id)), fullname(move(fullname)), email(move(email)), rentedDate(rentedDate), dueDate(dueDate) {
    }

    string userId;
    string fullname;
    string email;
    date rentedDate;
    date dueDate;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserInfoResponse, userId, fullname, email, rentedDate, dueDate);
};

#endif //USER_INFO_RESPONSE_HPP
