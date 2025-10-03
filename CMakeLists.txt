cmake_minimum_required(VERSION 3.15)
project(atm_virtual_banking_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(banklib
    src/BankAccount.cpp
    src/SavingAccount.cpp
    src/CheckingAccount.cpp
)
target_include_directories(banklib PUBLIC include)

add_executable(atm_app src/main.cpp)
target_link_libraries(atm_app PRIVATE banklib)

add_executable(basic_tests tests/basic_tests.cpp)
target_link_libraries(basic_tests PRIVATE banklib)
enable_testing()
add_test(NAME run_basic_tests COMMAND basic_tests)
