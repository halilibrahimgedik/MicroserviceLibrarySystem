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

    friend void to_json(json& json, const UserInfo& u) {
        json = nlohmann::json{
            {"userId", u.userId.to_string()},
            {"fullname", u.fullname},
            {"email", u.email},
            {"rentedDate", Utility::formatDate(u.rentedDate)},
            {"dueDate", Utility::formatDate(u.dueDate)}
        };
    }

    friend void from_json(const json& json, UserInfo& u) {
        u.userId = static_cast<bsoncxx::oid>(json.at("userId").get<string>());
        u.fullname = json.at("fullname").get<string>();
        u.email = json.at("email").get<string>();
    }
};


#endif //USER_HPP
