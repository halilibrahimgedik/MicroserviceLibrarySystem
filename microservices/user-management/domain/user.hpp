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
};

#endif //USER_HPP
