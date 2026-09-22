cmake_minimum_required(VERSION 3.20)

project(blogman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(blogman
    src/main.cpp
    src/views.cpp
    src/guestbook.cpp
)

target_include_directories(blogman PRIVATE include)