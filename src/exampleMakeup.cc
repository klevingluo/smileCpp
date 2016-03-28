#include <exampleMakeup.h>
#include "ThinPlateSpline/CThinPlateSpline.h"
#include <iostream>

using namespace std;
using namespace cv;

exampleMakeup::exampleMakeup(face example) 
: face(example.getImage(), example.getLandmarks()){
    }
                      
face exampleMakeup::applyTo(face model, double weight) {

  Mat resize;
  cv::resize(this->getImage(), resize, model.getImage().size(), 0, 0, INTER_NEAREST);

  vector<Point> old_landmarks = this->getLandmarks();
  vector<Point> dest_landmarks = vector<Point>(91);

  for (int i=0; i < old_landmarks.size(); i++) {
    double newx = old_landmarks[i].x * model.getImage().cols / this->getImage().cols;
    double newy = old_landmarks[i].y * model.getImage().rows / this->getImage().rows;
    dest_landmarks[i] = cv::Point(newx, newy);
  }


  CThinPlateSpline tps(dest_landmarks, model.getLandmarks());

  Mat example;
  tps.warpImage(resize, example, INTER_CUBIC, BACK_WARP);

  
  // converts the example to lab colorspace
  cv::cvtColor(example, example, cv::COLOR_BGR2Lab);

  // separates the lab color channels of the example image
  Mat *labChannels = new Mat[example.channels()];
  cv::split(example, labChannels);

  Mat inputLab;
  cv::cvtColor(model.getImage(), inputLab, cv::COLOR_BGR2Lab);

  Mat *InputLabChannels = new Mat[inputLab.channels()];

  cv::split(inputLab, InputLabChannels);
  face lay = face(example, model.getLandmarks());

  Mat anose = lay.getMask();
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

  return face(finalMat, model.getLandmarks());
}
