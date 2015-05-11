cmake_minimum_required(VERSION 2.8)

project(EasyCalculator)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(EasyCalculator expression.cpp commands.cpp parsers.cpp completion.cpp main.cpp)

target_link_libraries(EasyCalculator gmp gmpxx readline)
