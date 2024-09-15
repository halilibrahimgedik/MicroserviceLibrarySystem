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
        dtos/request/book/create-book-request.hpp
        dtos/response/book/create-book-response.hpp
        dtos/response/book/book-response.hpp
        dtos/response/user-info/user-info-response.hpp
        dtos/response/book/book-by-id-response.hpp
        dtos/request/book/update-book-request.hpp
        dtos/request/user-info/user-info-request.hpp
        dtos/response/book/book-list-response.hpp
)

target_link_libraries(library-management PRIVATE
        amqpcpp
        nlohmann_json::nlohmann_json
        $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
        $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>
)