cmake_minimum_required(VERSION 3.29)
project(library-management)

set(CMAKE_CXX_STANDARD 20)

find_package(amqpcpp CONFIG REQUIRED)

find_package(mongocxx CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(library-management main.cpp
        domain/book.hpp
        domain/book-repository.hpp
        domain/book-service.hpp
        domain/book-factory.hpp
        application/book-application-service.hpp
        domain/value-object/user-info.hpp
        message-listener.hpp
        infrustructure/db-connection.hpp)

target_link_libraries(library-management PRIVATE
        amqpcpp
        nlohmann_json::nlohmann_json
        $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
        $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>
)