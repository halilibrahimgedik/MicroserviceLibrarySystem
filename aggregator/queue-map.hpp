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
        {"user.update", ActionInfo{ {"user.update", "book.updateUserToBooks"}}}, // geriye data dönmesin
        {"user.delete", ActionInfo{{"user.delete", "book.deleteUserToBooks"}}}, // geriye data dönmemeli { "data": "","errors": null }
        {"user.getById", ActionInfo{{"user.getById"}}},

        {"book.getList", ActionInfo{{"book.getList"}}}, //! gelmiyor data
        {"book.insert", ActionInfo{{"book.insert"}}},
        {"book.update", ActionInfo{{"book.update"}}}, // geriye data dönmesin {"data": "book successfully updated", "errors": null }
        {"book.delete", ActionInfo{{"book.delete"}}},
        {"book.getById", ActionInfo{{"book.getById"}}},
        {"book.addUserToBook", ActionInfo{{"user.getById", "book.addUserToBook"}} },
    };
}

#endif //QUEUE_MAP_HPP
