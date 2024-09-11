cmake_minimum_required(VERSION 3.29)
project(aggregator)

set(CMAKE_CXX_STANDARD 20)

find_package(amqpcpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

add_executable(aggregator main.cpp
        queue-map.hpp
        aggregator-listener.hpp
        action-info.hpp
        ../infrastructure/response-dto.hpp
)

target_link_libraries(aggregator PRIVATE
        amqpcpp
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
)