#ifndef QUEUE_MAP_HPP
#define QUEUE_MAP_HPP

#include <string>
#include <unordered_map>

#include "action-info.hpp"

namespace QueueMap {
    using namespace std;


    inline unordered_map<string, ActionInfo> actionQueueMap {
        {"result", ActionInfo{{"gateway"}}},
        {"user.getList", ActionInfo{{"user.getList"}}},
        {"user.insert", ActionInfo{{"user.insert"}}},
        {"user.update", ActionInfo{ {"user.update"}}},
        {"user.delete", ActionInfo{{"user.getList"}}},
        {"user.getById", ActionInfo{{"user.getList"}}},

        {"book.getList", ActionInfo{{"user.getList"}}},
        {"book.insert", ActionInfo{{"user.getList"}}},
        {"book.update", ActionInfo{{"user.getList"}}},
        {"book.delete", ActionInfo{{"user.getList"}}},
        {"book.getById", ActionInfo{{"user.getList"}}},
        {"book.addUserToBook", ActionInfo{{"user.getById", "book.addAnUserToBook"}} },
    };


    // inline unordered_map<string, string> actionQueueMap {
    //     {"result", "gateway"},
    //     {"user.getList", "user.getList"},
    //     {"user.insert", "user.insert"},
    //     {"user.update", "user.update"},
    //     {"user.delete", "user.delete"},
    //     {"user.getById", "user.getById"},
    //     {"book.getList", "book.getList"},
    //     {"book.insert", "book.insert"},
    //     {"book.update", "book.update"},
    //     {"book.delete", "book.delete"},
    //     {"book.getById", "book.getById"},
    // };
}

#endif //QUEUE_MAP_HPP
