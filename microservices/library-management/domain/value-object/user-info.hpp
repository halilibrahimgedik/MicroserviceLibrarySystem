#ifndef USER_HPP
#define USER_HPP

#include <mongocxx/client.hpp>
#include <nlohmann/json.hpp>

#include "../../../../infrastructure/utility.hpp"

using namespace std;
using json = nlohmann::json;

class UserInfo {
    using date = chrono::system_clock::time_point;

public:
    UserInfo() = default;

    UserInfo(const bsoncxx::oid id, string fullname, string email, const date &rentedDate, const date &dueDate)
        : userId(id), fullname(move(fullname)), email(move(email)), rentedDate(rentedDate), dueDate(dueDate) {
    }

    bsoncxx::oid userId;
    string fullname;
    string email;
    date rentedDate;
    date dueDate;

};


#endif //USER_HPP
