#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <mongocxx/client.hpp>
#include <nlohmann/json.hpp>

using namespace std;

class User {
public:
    User() = default;
    User(string  fullname, string email): id(id), fullname(move(fullname)), email(move(email)){}
    User(const bsoncxx::oid id ,string  fullname, string email): id(id), fullname(move(fullname)), email(move(email)){}

    bsoncxx::oid id;
    string fullname;
    string email;

    // basic türler için macro çalışıyor, bsoncxx::oid kabul etmiyor
     NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, fullname, email)
};

#endif //USER_HPP
