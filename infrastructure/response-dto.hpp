#ifndef CUSTOM_RESPONSE_DTO_HPP
#define CUSTOM_RESPONSE_DTO_HPP

#include <string>
#include <utility>
#include <vector>
#include "nlohmann/json.hpp"
#include "nlohmann-extensions.hpp"

class ResponseDto {
public:
  ResponseDto() = default;
  ResponseDto(std::string action, std::string requestId, const int& index, const int& statusCode)
    : action(std::move(action)), requestId(std::move(requestId)), index(index), statusCode(statusCode) {}

  nlohmann::json jsonData;
  std::optional<std::vector<std::string>> errors;
  std::string action;
  std::string requestId;
  int index {0};
  int statusCode{0};


  [[nodiscard]] std::string to_string() const {
    const nlohmann::json jsonObj = *this;
    return jsonObj.dump();
  }


  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResponseDto, jsonData, errors, action, requestId, index, statusCode);
};

#endif // CUSTOM_RESPONSE_DTO_HPP