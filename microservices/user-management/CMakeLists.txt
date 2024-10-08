cmake_minimum_required(VERSION 3.29)
project(user-management)

set(CMAKE_CXX_STANDARD 20)

find_package(amqpcpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

find_package(mongocxx CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)
find_package(nlohmann_json  REQUIRED)


add_executable(user-management main.cpp
        domain/user.hpp
        domain/user-repository.hpp
        domain/user-factory.hpp
        domain/user-service.hpp
        application/user-application-service.hpp
        message-listener.hpp
        dtos/request/user/create-user-request.hpp
        ../../infrastructure/nlohmann-extensions.hpp
        dtos/response/user/user-response.hpp
        dtos/response/user/user-by-id-response.hpp
        dtos/request/user/update-user-request.hpp
        dtos/response/user/user-list-response.hpp
)

target_link_libraries(user-management PRIVATE
        amqpcpp
        nlohmann_json::nlohmann_json
        Boost::system
        OpenSSL::SSL
        OpenSSL::Crypto
        $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>
        $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>
)