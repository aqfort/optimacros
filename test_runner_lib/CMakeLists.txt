cmake_minimum_required(VERSION 3.20)

project(test_runner_lib)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} test_runner.h test_runner.cpp)
