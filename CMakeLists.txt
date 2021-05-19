cmake_minimum_required(VERSION 3.18)
project(Kulibot VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED)

add_executable(kulibot main.cpp
    bot/Kulibot.cpp
    commands/ping.cpp
    commands/boh.cpp
    utils/inventory.cpp
    utils/config.cpp
)

add_subdirectory(deps/DPP)
target_link_libraries(kulibot dpp)

add_subdirectory(deps/spdlog)
target_link_libraries(kulibot spdlog)

target_include_directories(kulibot PUBLIC .)
target_include_directories(kulibot PUBLIC deps/DPP/include)
target_include_directories(kulibot PUBLIC deps/spdlog/include)

configure_file(config.json ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)
