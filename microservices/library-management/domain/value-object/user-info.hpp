#ifndef USER_HPP
#define USER_HPP

#include <mongocxx/client.hpp>
#include <nlohmann/json.hpp>


using namespace std;
using json = nlohmann::json;

class UserInfo {
    using date = chrono::system_clock::time_point;

public:
    UserInfo() = default;

    UserInfo(const bsoncxx::oid& id, string fullname, string email, const date &rentedDate, const date &dueDate,
        const bool& isDelivered)
    :
    userId(id), fullname(move(fullname)), email(move(email)), rentedDate(rentedDate), dueDate(dueDate),
        isDelivered(isDelivered) {}

    bsoncxx::oid userId;
    string fullname;
    string email;
    date rentedDate;
    date dueDate;
    bool isDelivered {false};
};


#endif //USER_HPP
