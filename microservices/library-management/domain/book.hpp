#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>
#include <vector>
#include <mongocxx/client.hpp>
#include "value-object/user-info.hpp"
#include <chrono>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class Book {
    using date = chrono::system_clock::time_point;

public:
    Book() = default;
    Book(string name, string author, string summary, string imageUrl, const vector<UserInfo>& users = {})
        : name(move(name)), author(move(author)), summary(move(summary)),imageUrl(move(imageUrl)), users(users) {};

    Book(const bsoncxx::oid& id, string name, string author, string summary, string imageUrl, const vector<UserInfo>& users = {})
        : id(id), name(move(name)), author(move(author)), summary(move(summary)),imageUrl(move(imageUrl)), users(users){};


    bool operator==(const Book& other) const {
        return (name == other.name) && (author == other.author);
    }

    bool operator!=(const Book& other) const {
        return !(*this == other);
    }

    bsoncxx::oid id;
    string name;
    string author;
    string summary;
    string imageUrl;
    vector<UserInfo> users;
};



#endif //BOOK_HPP
