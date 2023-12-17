cmake_minimum_required(VERSION 3.27)

project(
    ftp
    VERSION 0.0.1
    DESCRIPTION "ftp client"
    LANGUAGES CXX)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ftp src/main.cpp src/client.cpp src/url.cpp)
