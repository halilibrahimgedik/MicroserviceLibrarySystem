#ifndef QUEUE_MAP_HPP
#define QUEUE_MAP_HPP

#include <string>
#include <unordered_map>

namespace QueueMap {
    using namespace std;

    const string userQueue{"userQueue"};
    const string gatewayQueue{"gatewayQueue"};

    inline unordered_map<string, string> actionQueueMap {
        {"result", gatewayQueue},
        {"user.getList", "user.getList"},
        {"user.insert", "user.insert"},
        {"user.update", "user.update"},
        {"user.delete", "user.delete"},
        {"user.getById", "user.getById"},
        {"book.getList", "book.getList"},
        {"book.insert", "book.insert"},
        {"book.update", "book.update"},
        {"book.delete", "book.delete"},
        {"book.getById", "book.getById"},
    };
}

#endif //QUEUE_MAP_HPP
