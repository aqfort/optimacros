cmake_minimum_required(VERSION 3.24)
project(mtfind)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -pthread)



include_directories(test_runner_lib profile)
add_subdirectory(test_runner_lib)



file(GLOB SOURCE source/*)



find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} Threads::Threads test_runner_lib)
