cmake_minimum_required(VERSION 3.15)
project(atm_virtual_banking CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Strong warnings + optimization
add_compile_options(-O2 -Wall -Wextra -Wpedantic)

# Header-only "engine" + "include"
add_library(bank_engine INTERFACE)
target_include_directories(bank_engine
    INTERFACE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/engine
)

# CLI app
add_executable(atm_app src/main.cpp)
target_link_libraries(atm_app PRIVATE bank_engine)

# Tests
add_executable(unit_test        tests/unit_test.cpp)
target_link_libraries(unit_test PRIVATE bank_engine)

add_executable(basic_tests      tests/basic_tests.cpp)
target_link_libraries(basic_tests PRIVATE bank_engine)

add_executable(fuzz_test        tests/fuzz_test.cpp)
target_link_libraries(fuzz_test PRIVATE bank_engine)

add_executable(scale_test       tests/scale_test.cpp)
target_link_libraries(scale_test PRIVATE bank_engine)

find_package(Threads REQUIRED)
add_executable(concurrency_test tests/concurrency_test.cpp)
target_link_libraries(concurrency_test PRIVATE bank_engine Threads::Threads)

# CTest wiring
include(CTest)
enable_testing()
add_test(NAME unit_test   COMMAND unit_test)
add_test(NAME basic_tests COMMAND basic_tests)
add_test(NAME fuzz_test   COMMAND fuzz_test)
add_test(NAME scale_test  COMMAND scale_test)
