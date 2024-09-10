#ifndef USER_FACTORY_HPP
#define USER_FACTORY_HPP


#include "user.hpp"
#include "../dtos/request/user/create-user-request-dto.hpp"
#include "../dtos/response/user/result-user-by-id-response-dto.hpp"
#include "../dtos/response/user/result-user-response-dto.hpp"

using namespace std;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

namespace UserFactory {

    bsoncxx::builder::basic::document inline generateUser(const CreateUserRequestDto& dto) {
        bsoncxx::builder::basic::document filter{};
        filter.append(
            kvp("fullname", dto.fullname),
            kvp("email", dto.email),
            kvp("isActive", true));

        return filter;
    }

    ResultUserByIdResponseDto inline generateUserById(const bsoncxx::document::value& docValue) {
        ResultUserByIdResponseDto user;
        user.userId = docValue["_id"].get_oid().value.to_string();
        user.fullname = docValue["fullname"].get_string().value;
        user.email = docValue["email"].get_string().value;
        user.isActive = docValue["isActive"].get_bool().value;

        return user;
    }

    vector<ResultUserResponseDto> inline generateUserList(mongocxx::cursor& cursor) {
        vector<ResultUserResponseDto> users;

        for(auto& doc : cursor) {
            ResultUserResponseDto user;
            user.userId = doc["_id"].get_oid().value.to_string();
            user.fullname = doc["fullname"].get_string().value;
            user.email = doc["email"].get_string().value;
            user.isActive = doc["isActive"].get_bool().value;
            users.push_back(std::move(user));
        }

        return users;
    }
}


#endif //USER_FACTORY_HPP
