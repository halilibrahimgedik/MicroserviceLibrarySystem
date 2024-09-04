cmake_minimum_required(VERSION 3.29)
project(api-gateway)

set(CMAKE_CXX_STANDARD 20)

find_package(Crow CONFIG REQUIRED)
find_package(amqpcpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)


add_executable(api-gateway main.cpp
        ../infrastructure/rabbit-mq-adapter.hpp
        ../infrastructure/rabbit-mq-connection-handler.hpp
        ../infrastructure/utility.hpp
        ../libs/libcpp-event-hub/src/libcpp-event-hub.hpp
        http-listener.hpp
)

target_link_libraries(api-gateway PRIVATE
        Crow::Crow
        amqpcpp
        nlohmann_json::nlohmann_json
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
)