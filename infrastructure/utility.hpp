#ifndef RANDOM_NUMBER_GENERATOR_HPP
#define RANDOM_NUMBER_GENERATOR_HPP

#include <string>
#include <random>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>

using namespace std;

namespace Utility {

    std::string inline generateUUID() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        unsigned char data[16];
        for (int i = 0; i < 16; ++i) {
            data[i] = static_cast<unsigned char>(dis(gen));
        }

        // Set version to 4 (random UUID)
        data[6] = (data[6] & 0x0F) | 0x40;
        // Set variant to 10xx
        data[8] = (data[8] & 0x3F) | 0x80;

        std::ostringstream oss;
        for (int i = 0; i < 16; ++i) {
            if (i == 4 || i == 6 || i == 8 || i == 10) {
                oss << '-';
            }
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
        }
        return oss.str();
    }

    nlohmann::json inline getMessage(const string_view::iterator& data, const size_t& size) {
        const std::string message(data, size);
        return nlohmann::json::parse(message);
    }

    chrono::system_clock::time_point inline parseDate(const std::string& dateStr) {
        std::tm tm = {};
        std::istringstream ss(dateStr);

        ss >> std::get_time(&tm, "%d.%m.%Y");
        if (ss.fail()) {
            throw std::invalid_argument("Unsupported date format");
        }

        const std::time_t time = std::mktime(&tm);
        if (time == -1) {
            throw std::runtime_error("Failed to convert time");
        }

        return std::chrono::system_clock::from_time_t(time);
    }

    string inline formatDate(const chrono::system_clock::time_point& timePoint) {
        const auto timeT = chrono::system_clock::to_time_t(timePoint);
        const std::tm tm = *std::localtime(&timeT);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%d.%m.%Y", &tm);
        return string{buffer};
    }
}

#endif //RANDOM_NUMBER_GENERATOR_HPP
