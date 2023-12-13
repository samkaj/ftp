cmake_minimum_required(VERSION 3.27)

project(
    tcp_client
    VERSION 0.0.1
    DESCRIPTION "TCP client"
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(ftp src/main.cpp)
