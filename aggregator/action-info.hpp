#ifndef ACTION_INFO_HPP
#define ACTION_INFO_HPP
#include <string>
#include <vector>


class ActionInfo {
public:
    ActionInfo(const std::vector<std::string>& queues) : queues(queues) {};

    std::vector<std::string> queues;
};

#endif //ACTION_INFO_HPP
