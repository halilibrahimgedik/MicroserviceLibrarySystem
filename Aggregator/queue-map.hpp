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
        {"getUserList", userQueue},
        {"insertUser", userQueue},
        {"updateUser", userQueue},
        {"deleteUser", userQueue},
        {"getUserById", userQueue},
        {"getBookList", userQueue},
        {"insertBook", userQueue},
        {"updateBook", userQueue},
        {"deleteBook", userQueue},
        {"getBookById", userQueue},
    };
}

#endif //QUEUE_MAP_HPP
