#include <string>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "ThinPlateSpline/CThinPlateSpline.h"
#include "layers.cc"

class makeup {

  public: 
    makeup() {
    }

    cv::Mat TransferMakeup(Mat input_image, vector<Point> input_landmarks, 
                           Mat example_image, vector<Point> example_landmarks) {

    Mat exampleRaw = example_image.clone();
    Mat input = input_image.clone();
    Mat example;

    Mat resize;
    cv::resize(exampleRaw, resize, input.size(), 0, 0, INTER_NEAREST);

    vector<Point> dest_landmarks = vector<Point>(83);
    for (int i=0; i < example_landmarks.size(); i++) {
      double newx = example_landmarks[i].x * input.cols / exampleRaw.cols;
      double newy = example_landmarks[i].y * input.rows / exampleRaw.rows;
      dest_landmarks[i] = cv::Point(newx, newy);
    }

    CThinPlateSpline tps(dest_landmarks, input_landmarks);

    tps.warpImage(resize, example, INTER_CUBIC, BACK_WARP);
    
    // converts the example to lab colorspace
    cv::cvtColor(example, example, cv::COLOR_BGR2Lab);
  
    // separates the lab color channels of the example image
    Mat *labChannels = new Mat[example.channels()];
    cv::split(example, labChannels);

    Mat inputLab;
    cv::cvtColor(input, inputLab, cv::COLOR_BGR2Lab);
  
    Mat *InputLabChannels = new Mat[inputLab.channels()];
  
    cv::split(inputLab, InputLabChannels);
    layers lay = layers(example, input_landmarks);

    Mat anose = lay.getShapes();
    Mat bnose = anose*(-1) + 255;

    Mat finalL = InputLabChannels[0].clone();
    Mat finalA;
    cv::addWeighted(InputLabChannels[1].mul(bnose)/255, 1, 
                  labChannels[1].mul(anose)/255, 1, 0, finalA);
    Mat finalB;
    cv::addWeighted(InputLabChannels[2].mul(bnose)/255, 1, 
                  labChannels[2].mul(anose)/255, 1, 0, finalB);

    Mat finalimage[] = {finalL.clone(), finalA.clone(), finalB.clone()};

    Mat finalMat;

    cv::merge(finalimage, 3, finalMat);
    cv::cvtColor(finalMat, finalMat, cv::COLOR_Lab2BGR);

    return finalMat;
  }
};
