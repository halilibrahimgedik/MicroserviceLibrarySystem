cmake_minimum_required(VERSION 3.29)
project(MicroLibSystem)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_TOOLCHAIN_FILE "/home/halil/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Alt projeler
add_subdirectory(api-gateway)
add_subdirectory(aggregator)
add_subdirectory(microservices/user-management)

