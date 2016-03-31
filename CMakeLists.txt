cmake_minimum_required (VERSION 2.8.4)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project
project(Makeup CXX)

# version number
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)

add_subdirectory(tests)
add_subdirectory(src)


