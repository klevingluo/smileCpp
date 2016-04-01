project(makeupTests CXX)
find_package(OpenCV REQUIRED)

add_subdirectory(images)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_SOURCE_DIR}/include/)

file(GLOB SRCS ${CMAKE_SOURCE_DIR}/src/*.cc 
               ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB TPS ${CMAKE_SOURCE_DIR}/include/ThinPlateSpline/*)

add_executable(differenceTest ./differenceMakeupTest.cc ${SRCS} ${TPS})
target_link_libraries(differenceTest ${OpenCV_LIBS})

add_executable(faceTest ./faceTest.cc ${SRCS} ${TPS})
target_link_libraries(faceTest ${OpenCV_LIBS})

add_executable(makeupTest ./makeupTest.cc ${SRCS} ${TPS})
target_link_libraries(makeupTest ${OpenCV_LIBS})

add_executable(softLightBlendTest ./softLightBlendTest.cc ${SRCS} ${TPS})
target_link_libraries(softLightBlendTest ${OpenCV_LIBS})

add_executable(foundationTest ./foundationTest.cc ${SRCS} ${TPS})
target_link_libraries(foundationTest ${OpenCV_LIBS})
