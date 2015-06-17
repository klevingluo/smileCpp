#!/bin/bash
g++ -g -o maskTest.o ./mask.cc ./face.h ./face.cc \
-std=c++11 \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-L./ThinPlateSpline/ \
-g\

./maskTest.o
