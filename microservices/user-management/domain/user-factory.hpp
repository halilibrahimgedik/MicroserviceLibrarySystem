#ifndef USER_FACTORY_HPP
#define USER_FACTORY_HPP


#include "user.hpp"
#include "../dtos/request/user/create-user-request.hpp"
#include "../dtos/response/user/user-by-id-response.hpp"
#include "../dtos/response/user/user-response.hpp"

using namespace std;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

namespace UserFactory {

    bsoncxx::builder::basic::document inline generateUser(const string& fullname, const string& email) {
        bsoncxx::builder::basic::document filter{};
        filter.append(
            kvp("fullname", fullname),
            kvp("email", email),
            kvp("isActive", true));

        return filter;
    }

    User inline generateUserById(const bsoncxx::document::value& docValue) {
        User user;
        user.userId = docValue["_id"].get_oid().value;
        user.fullname = docValue["fullname"].get_string().value;
        user.email = docValue["email"].get_string().value;
        user.isActive = docValue["isActive"].get_bool().value;

        return user;
    }

    vector<User> inline generateUserList(mongocxx::cursor& cursor) {
        vector<User> users;

        for(auto& doc : cursor) {
            User user;
            user.userId = doc["_id"].get_oid().value;
            user.fullname = doc["fullname"].get_string().value;
            user.email = doc["email"].get_string().value;
            user.isActive = doc["isActive"].get_bool().value;
            users.push_back(std::move(user));
        }

        return users;
    }
}


#endif //USER_FACTORY_HPP
