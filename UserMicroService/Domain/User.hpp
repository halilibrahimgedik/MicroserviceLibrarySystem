#ifndef USER_HPP
#define USER_HPP

#include <optional>
#include <string>
#include <bsoncxx/oid.hpp>

using namespace std;

class User {
public:
    User() = default;

    bsoncxx::oid id;
    string fullname;
    string email;

};

#endif //USER_HPP
