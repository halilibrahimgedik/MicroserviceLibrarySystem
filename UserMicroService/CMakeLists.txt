cmake_minimum_required(VERSION 3.29)
project(UserMicroService)

set(CMAKE_CXX_STANDARD 20)

find_package(amqpcpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

find_package(mongocxx CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)


add_executable(UserMicroService main.cpp
        Domain/user.hpp
        Domain/user-repository.hpp
        Domain/user-factory.hpp
        Domain/user-service.hpp
        Infrastructure/db-connection.hpp
        Application/user-application-service.hpp)

target_link_libraries(UserMicroService PRIVATE
        amqpcpp
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
        $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>
        $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
)