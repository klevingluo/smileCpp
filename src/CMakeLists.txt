CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project(makeup CXX)
find_package(OpenCV REQUIRED)

set(CMake_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11")

add_compile_options(
-std=c++11
-lopencv_core 
-lopencv_highgui 
-lopencv_imgproc)

file(GLOB SRCS *.cc *.h)
file(GLOB TPS ./ThinPlateSpline/*)
