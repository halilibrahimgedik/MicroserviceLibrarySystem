#ifndef CONFIGURATION_HELPER_HPP
#define CONFIGURATION_HELPER_HPP


#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using namespace std;

class ConfigurationHelper {
private:
    ConfigurationHelper() { loadAppConfigFile(); }
    nlohmann::json jsonData;

    void loadAppConfigFile(const string& filePath = R"(/home/halil/Desktop/Projects/MicroLibSystem/aggregator/app-config.json)")
    {
        if (ifstream file{filePath}; file.is_open()) {
            file >> jsonData; // dosyayı json nesnesine aktaralım
        } else {
            cerr << "Error occurred while trying to open file: " << filePath << endl;
        }
    }

public:
    // Kopyalama ve atama işlemlerini engelle
    ConfigurationHelper(const ConfigurationHelper&) = delete;
    ConfigurationHelper& operator=(const ConfigurationHelper&) = delete;

    static ConfigurationHelper& getInstance() {
        static ConfigurationHelper instance;
        return instance;
    }

    [[nodiscard]] string getValue(const string& key) const {
        if (!jsonData.contains(key)) {
            return "";
        }

        return jsonData[key].get<string>();
    }
};


#endif //CONFIGURATION_HELPER_HPP
