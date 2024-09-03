#ifndef RANDOM_NUMBER_GENERATOR_HPP
#define RANDOM_NUMBER_GENERATOR_HPP

#include <string>
#include <random>
#include <iostream>
#include <iomanip>
#include <sstream>

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
}

#endif //RANDOM_NUMBER_GENERATOR_HPP
