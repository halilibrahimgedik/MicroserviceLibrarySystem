#ifndef USER_FACTORY_HPP
#define USER_FACTORY_HPP


#include "user.hpp"

using namespace std;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;
using namespace std;

namespace UserFactory {

    bsoncxx::builder::basic::document inline generateUser(const User &user) {
        bsoncxx::builder::basic::document filter{};
        filter.append(
            kvp("fullname", user.fullname),
            kvp("email", user.email));

        return filter;
    }

    User inline generateUserById(const bsoncxx::document::value& docValue) {
        User user;
        user.id = docValue["_id"].get_oid().value;
        user.fullname = docValue["fullname"].get_string().value;
        user.email = docValue["email"].get_string().value;

        return user;
    }

    vector<User> inline generateUserList(mongocxx::cursor& cursor) {
        vector<User> users;

        for(auto& doc : cursor) {
            User user;
            user.id = doc["_id"].get_oid().value;
            user.fullname = doc["fullname"].get_string().value;
            user.email = doc["email"].get_string().value;
            users.push_back(std::move(user));
        }

        return users;
    }
}


#endif //USER_FACTORY_HPP
