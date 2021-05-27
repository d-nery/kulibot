cmake_minimum_required(VERSION 3.18)
project(Kulibot VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(COMPILE_TEST False)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og")

add_executable(kulibot main.cpp
    bot/Kulibot.cpp
    commands/ping.cpp
    commands/boh.cpp
    commands/char.cpp
    commands/roll.cpp
    commands/reload.cpp
    utils/characters.cpp
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

add_custom_command(TARGET kulibot PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:kulibot>/data)
if(COMPILE_TEST)
  # enable_testing()

  add_executable(kuliteste
    tests/inventory_test.cpp
    tests/config_test.cpp
    tests/char_test.cpp

    utils/inventory.cpp
    utils/config.cpp
    utils/characters.cpp
  )

  add_subdirectory(deps/googletest)
  target_link_libraries(kuliteste gtest_main)

  target_include_directories(kuliteste PUBLIC .)
  target_include_directories(kuliteste PUBLIC deps/DPP/include)
  target_include_directories(kuliteste PUBLIC deps/spdlog/include)

  add_custom_command(TARGET kuliteste POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                         ${CMAKE_SOURCE_DIR}/tests/testdata $<TARGET_FILE_DIR:kuliteste>/testdata)

  include(GoogleTest)
  gtest_discover_tests(kuliteste)
endif()
