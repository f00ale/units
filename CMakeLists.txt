cmake_minimum_required(VERSION 3.10)
project(playground)

set(CMAKE_CXX_STANDARD 17)

add_library(units INTERFACE)
target_include_directories(units INTERFACE include)

add_executable(units_test test/main.cpp)

target_link_libraries(units_test PRIVATE units)
