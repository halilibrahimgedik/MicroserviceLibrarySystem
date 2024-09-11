#ifndef CUSTOM_RESPONSE_DTO_HPP
#define CUSTOM_RESPONSE_DTO_HPP

#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "nlohmann-extensions.hpp"

class ResponseDto {
public:
  ResponseDto() = default;

  nlohmann::json jsonData;
  std::optional<std::vector<std::string>> errors;
  std::string requestId;
  std::string action;
  int index = 0;


  [[nodiscard]] std::string to_string() const {
    const nlohmann::json jsonObj = *this;
    return jsonObj.dump(4);
  }


  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResponseDto, jsonData, errors, action, requestId, index);
};

#endif // CUSTOM_RESPONSE_DTO_HPP