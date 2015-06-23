CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project(makeupTests CXX)
find_package(OpenCV REQUIRED)

set(CMake_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11")

add_compile_options(
-std=c++11
-lopencv_core 
-lopencv_highgui 
-lopencv_imgproc)

file(GLOB SRCS ../src/*.cc ../src/*.h)
file(GLOB TPS ../src/ThinPlateSpline/*)

add_executable(faceTest ./faceTest.cc ${SRCS} ${TPS})
target_link_libraries(faceTest ${OpenCV_LIBS})

add_executable(Test ./test.cc ${SRCS} ${TPS})
target_link_libraries(Test ${OpenCV_LIBS})

add_executable(maskTest ./mask.cc ${SRCS} ${TPS})
target_link_libraries(maskTest ${OpenCV_LIBS})
