#ifndef USER_LIST_RESPONSE_HPP
#define USER_LIST_RESPONSE_HPP
#include <vector>
#include <nlohmann/detail/macro_scope.hpp>

#include "user-response.hpp"
#include "../../../../../infrastructure/nlohmann-extensions.hpp"

class UserListResponse {
public:
  UserListResponse() = default;


  std::vector<UserResponse> users;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(UserListResponse, users);
};

#endif //USER_LIST_RESPONSE_HPP
