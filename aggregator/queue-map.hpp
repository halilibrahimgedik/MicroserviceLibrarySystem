#ifndef QUEUE_MAP_HPP
#define QUEUE_MAP_HPP

#include <string>
#include <unordered_map>

#include "action-info.hpp"

namespace QueueMap {
    using namespace std;


    inline unordered_map<string, ActionInfo> actionQueueMap {
        {"user.getList", ActionInfo{{"user.getList"}}},
        {"user.insert", ActionInfo{{"user.insert"}}},
        {"user.update", ActionInfo{ {"user.update"}}},
        {"user.delete", ActionInfo{{"user.delete"}}},
        {"user.getById", ActionInfo{{"user.getById"}}},

        {"book.getList", ActionInfo{{"book.getList"}}},
        {"book.insert", ActionInfo{{"book.insert"}}},
        {"book.update", ActionInfo{{"book.update"}}},
        {"book.delete", ActionInfo{{"book.delete"}}},
        {"book.getById", ActionInfo{{"book.getById"}}},
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
