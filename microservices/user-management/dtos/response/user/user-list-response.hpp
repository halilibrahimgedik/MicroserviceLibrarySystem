#ifndef USER_LIST_RESPONSE_HPP
#define USER_LIST_RESPONSE_HPP

#include <vector>
#include "user-response.hpp"

class UserListResponse {
public:
  UserListResponse() = default;


  std::vector<UserResponse> users;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserListResponse, users);
};

#endif //USER_LIST_RESPONSE_HPP
