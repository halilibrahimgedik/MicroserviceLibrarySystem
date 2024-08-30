cmake_minimum_required(VERSION 3.29)
project(UserMicroService)

set(CMAKE_CXX_STANDARD 20)

find_package(amqpcpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

find_package(mongocxx CONFIG REQUIRED)

add_executable(UserMicroService main.cpp
        Domain/User.hpp)

target_link_libraries(UserMicroService PRIVATE
        amqpcpp
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
        mongocxx
        bsoncxx
)