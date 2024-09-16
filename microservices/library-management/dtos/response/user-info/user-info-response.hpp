#ifndef USER_INFO_RESPONSE_HPP
#define USER_INFO_RESPONSE_HPP

#include <chrono>
#include <string>

using namespace std;

class UserInfoResponse {
public:
    UserInfoResponse() = default;
    UserInfoResponse(string id, string fullname, string email, string rentedDate, string dueDate, const bool& isDelivered)
    : userId(move(id)), fullname(move(fullname)), email(move(email)), rentedDate(std::move(rentedDate)), dueDate(std::move(dueDate)), isDelivered(isDelivered) {}

    string userId;
    string fullname;
    string email;
    string rentedDate;
    string dueDate;
    bool isDelivered {false};

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserInfoResponse, userId, fullname, email, rentedDate, dueDate);
};

#endif //USER_INFO_RESPONSE_HPP
