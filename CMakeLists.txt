cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

project(CryptoWatch)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

add_executable(CryptoWatch src/main.cpp)