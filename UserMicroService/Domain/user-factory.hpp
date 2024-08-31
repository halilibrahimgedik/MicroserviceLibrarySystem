#ifndef USER_FACTORY_HPP
#define USER_FACTORY_HPP

#include <string>

#include "user.hpp"
using namespace std;

namespace UserFactory {

    bsoncxx::builder::basic::document inline generateUser(const User &user) {
        bsoncxx::builder::basic::document filter{};
        filter.append(
            kvp("fullname", user.fullname),
            kvp("email", user.email));

        return filter;
    }

    User inline getUserById(const bsoncxx::document::value& docValue) {
        User user;
        user.id = docValue["_id"].get_oid().value;
        user.fullname = docValue["fullname"].get_string().value;
        user.email = docValue["email"].get_string().value;

        return user;
    }
}


#endif //USER_FACTORY_HPP
