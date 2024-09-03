#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <mongocxx/client.hpp>
#include <utility>
#include <nlohmann/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

using namespace std;
using json = nlohmann::json;

class User {
public:
    User() = default;
    User(string  fullname, string email): id(id), fullname(move(fullname)), email(move(email)){}
    User(const bsoncxx::oid id ,string  fullname, string email): id(id), fullname(move(fullname)), email(move(email)){}

    bsoncxx::oid id;
    string fullname;
    string email;


    // JSON serileştirme
    [[nodiscard]] json toJson() const {
        return json{
            {"id", id.to_string()},
            {"fullname", fullname},
            {"email", email}};
    }

    // JSON'dan user elde etme
    static User fromJson(const json& j) {
        User user;
        user.id = bsoncxx::oid{j.at("id").get<string>()};
        user.fullname = j.at("fullname").get<string>();
        user.email =  j.at("email").get<string>();
        return user;
    }
};

#endif //USER_HPP
