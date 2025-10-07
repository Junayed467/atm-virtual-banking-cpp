cmake_minimum_required(VERSION 3.15)
project(concurrent_bank_engine CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O2 -Wall -Wextra -Wpedantic)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/engine)

add_executable(unit_test        tests/unit_test.cpp)
add_executable(fuzz_test        tests/fuzz_test.cpp)
add_executable(scale_test       tests/scale_test.cpp)
add_executable(concurrency_test tests/concurrency_test.cpp)

find_package(Threads REQUIRED)
target_link_libraries(concurrency_test Threads::Threads)
