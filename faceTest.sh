#!/bin/bash
g++ -g -o faceTest.o ./faceTest.cc ./face.cc  \
-std=c++11 \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-L./ThinPlateSpline/ \
-g\

./faceTest
