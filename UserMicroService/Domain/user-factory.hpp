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
}


#endif //USER_FACTORY_HPP
