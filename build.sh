#!/bin/bash
g++ -g -o bin read.cpp \
-std=c++11 \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-L./ThinPlateSpline/ \
-g\
