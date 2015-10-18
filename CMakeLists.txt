cmake_minimum_required (VERSION 2.8.4)

# project
project(Makeup CXX)

# version number
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)

add_definitions(-D DEF_REST_1)

message("Binary Tree path: ${PROJECT_BINARY_DIR}")

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(images)
