#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <mongocxx/client.hpp>
#include <nlohmann/json.hpp>

using namespace std;
class User {
public:
    User() = default;
    User(string  fullname, string email, const bool& isActive = true)
        : userId(userId), fullname(move(fullname)), email(move(email)), isActive(isActive){}
    User(const bsoncxx::oid userId ,string  fullname, string email, const bool& isActive = true)
        : userId(userId), fullname(move(fullname)), email(move(email)), isActive(isActive){}

    bsoncxx::oid userId;
    string fullname;
    string email;
    bool isActive {true};

    friend void to_json(nlohmann::json& json, const User& user){
        json = nlohmann::json{
            {"userId", user.userId.to_string()},
            {"fullname", user.fullname},
            {"email", user.email},
            {"isActive", user.isActive}
        };
    }

    friend void from_json(const nlohmann::json& json, User& user) {
        user.userId = static_cast<bsoncxx::oid>(json["userId"].get<string>());
        user.fullname = json["fullname"].get<string>();
        user.email = json["email"].get<string>();
        user.isActive = json["isActive"].get<bool>();
    }

    // basic türler için macro çalışıyor, bsoncxx::oid kabul etmiyor
     // NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, fullname, email)
};

#endif //USER_HPP
