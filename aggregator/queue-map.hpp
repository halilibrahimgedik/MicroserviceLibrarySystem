#ifndef QUEUE_MAP_HPP
#define QUEUE_MAP_HPP

#include <string>
#include <unordered_map>

#include "action-info.hpp"

namespace QueueMap {
    using namespace std;


    inline unordered_map<string, ActionInfo> actionQueueMap {
        {"user-management.getList", ActionInfo{{"user-management.getList"}}},
        {"user-management.insert", ActionInfo{{"user-management.insert"}}},
        {"user-management.update", ActionInfo{ {"user-management.update", "library-management.updateUserToBooks"}}},
        {"user-management.delete", ActionInfo{{"user-management.delete", "library-management.deleteUserToBooks"}}},
        {"user-management.getById", ActionInfo{{"user-management.getById"}}},


        {"library-management.getList", ActionInfo{{"library-management.getList"}}},
        {"library-management.insert", ActionInfo{{"library-management.insert"}}},
        {"library-management.update", ActionInfo{{"library-management.update"}}},
        {"library-management.delete", ActionInfo{{"library-management.delete"}}},
        {"library-management.getById", ActionInfo{{"library-management.getById"}}},
        {"library-management.getByIdWithUsers", ActionInfo{{"library-management.getByIdWithUsers"}}},
        {"library-management.getBooksWithUsers", ActionInfo{{"library-management.getBooksWithUsers"}}},

        {"library-management.rentBook", ActionInfo{{"user-management.getById", "library-management.rentBook"}}},
        {"library-management.deliverBook", ActionInfo{{"library-management.deliverBook"}}},
        {"library-management.getUserBookList", ActionInfo{{"library-management.getUserBookList"}}},
    };
}

#endif //QUEUE_MAP_HPP
