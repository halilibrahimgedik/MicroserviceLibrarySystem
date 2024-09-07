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
    Book(string name, string author, const vector<UserInfo>& users)
        : id(id), name(move(name)), author(move(author)), users(users) {
    };
    Book(const bsoncxx::oid& id, string name, string author, const vector<UserInfo>& users)
        : id(id), name(move(name)), author(move(author)), users(users) {
    };

    // Karşılaştırma operatörünü overload
    bool operator==(const Book& other) const {
        return (name == other.name) && (author == other.author);
    }

    bool operator!=(const Book& other) const {
        return !(*this == other);
    }

    bsoncxx::oid id;
    string name;
    string author;
    vector<UserInfo> users;

    friend void to_json(json& json, const Book& b) {
        json = nlohmann::json{
            {"id", b.id.to_string()},
            {"name", b.name},
            {"author", b.author},
            {"users", b.users}
        };
    }

    friend void from_json(const json& json, Book& b) {
        b.id = bsoncxx::oid{json.at("id").get<string>()};
        json.at("name").get_to(b.name);
        json.at("author").get_to(b.author);
        b.users = json.at("users").get<vector<UserInfo>>();
    }

    // NLOHMANN_DEFINE_TYPE_INTRUSIVE(Book, name, author, id, users);
};



#endif //BOOK_HPP
