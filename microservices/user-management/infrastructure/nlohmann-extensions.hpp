#ifndef NLOHMANN_EXTENSIONS_HPP
#define NLOHMANN_EXTENSIONS_HPP

#include <nlohmann/json.hpp>
#include <optional>
#include <iomanip>
#include <sstream>
#include <string>


inline std::string convertToIsoFormat(std::chrono::time_point<std::chrono::system_clock> timePoint)
{
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);

    std::stringstream ss;

    ss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%SZ");

    return ss.str();
}

inline std::chrono::system_clock::time_point convertFromIsoFormat(const std::string& isoFormat)
{
    std::tm tm = {};
    std::istringstream ss(isoFormat);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}


namespace nlohmann
{
    template <typename T>
    void to_json(json& j, const std::optional<T>& opt)
    {
        if (opt.has_value())
        {
            j = *opt;
        }
        else
        {
            j = nullptr;
        }
    }

    template <typename T>
    void from_json(const json& j, std::optional<T>& opt)
    {
        if (j.is_null())
        {
            opt = std::nullopt;
        }
        else
        {
            opt = j.get<T>();
        }
    }

    void to_json(json& j, const std::chrono::system_clock::time_point& tp)
    {
        j = convertToIsoFormat(tp);
    }

    void from_json(const json& j, std::chrono::system_clock::time_point& tp)
    {
        tp = convertFromIsoFormat(j.get<std::string>());
    }

}

#endif //NLOHMANN_EXTENSIONS_HPP
