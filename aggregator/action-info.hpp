#ifndef ACTION_INFO_HPP
#define ACTION_INFO_HPP
#include <string>
#include <vector>


class ActionInfo {
public:
    ActionInfo(const std::vector<std::string>& queues) : queues(queues) {};

    std::vector<std::string> queues;

    // std::optional<std::string> responseMessage;
    // int statusCode;
};

#endif //ACTION_INFO_HPP
