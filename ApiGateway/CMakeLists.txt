cmake_minimum_required(VERSION 3.29)
project(ApiGateway)

set(CMAKE_CXX_STANDARD 20)

find_package(Crow CONFIG REQUIRED)
find_package(amqpcpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)


add_executable(ApiGateway main.cpp)

target_link_libraries(ApiGateway PRIVATE
        Crow::Crow
        amqpcpp
        nlohmann_json::nlohmann_json
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
)