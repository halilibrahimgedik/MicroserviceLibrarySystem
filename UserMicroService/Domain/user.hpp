#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <mongocxx/client.hpp>
#include <utility>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

using namespace std;

class User {
public:
    User() = default;
    User(string  fullname, string email): id(id), fullname(move(fullname)), email(move(email)){}
    User(const bsoncxx::oid id ,string  fullname, string email): id(id), fullname(move(fullname)), email(move(email)){}

    bsoncxx::oid id;
    string fullname;
    string email;
};

#endif //USER_HPP
