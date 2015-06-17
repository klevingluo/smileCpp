#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ThinPlateSpline/CThinPlateSpline.cpp"
#include "./face.h"

using namespace cv;
using namespace std;

face process(face f);
face warpTo(face from, face to);

int main() {
  face makeup("./images/A*.png", "./images/A*.txt");
  face nomakeup("./images/A.png", "./images/A.txt");

  face plus = process(makeup);
  face minus = process(warpTo(nomakeup,makeup));

  face mask(plus.getImage() - minus.getImage(), 
            plus.getLandmarks());

  imshow("mask",mask.getImage());

  //face input("./images/B.jpg", "./images/B.txt");
  face input("./images/A.png", "./images/A.txt");

  face contouredMask = warpTo(mask, input);

  imshow("final",contouredMask.getImage() + input.getImage());

  waitKey(0);
  return 0;
}

face process(face f) {
  Mat mask = f.getImage();

  Mat removals = 255 - 
                 f.getFeature(face::FOREHEAD) -
                 f.getFeature(face::HEAD) + 
                 f.getFeature(face::LEFT_EYE) + 
                 f.getFeature(face::RIGHT_EYE) +
                 f.getFeature(face::LEFT_BROW) + 
                 f.getFeature(face::RIGHT_BROW);

  vector<Mat> layers;
  cv::split(mask, layers);

  for(int i=0; i<layers.size(); i++) {
    layers[i] = layers[i] - removals;
  }

  cv::merge(layers, mask);

  face m(mask, f.getLandmarks());
  return m;
}

face warpTo(face from, face to) {
  Mat warpedFrom;
  Mat scaledFrom;
  cv::resize(from.getImage(), scaledFrom, to.getImage().size(), 
             0, 0, INTER_NEAREST);

  vector<Point> scaledLandmarks = vector<Point>(from.getLandmarks().size());

  double xScale = (0.0 + to.getImage().cols) / from.getImage().cols;
  double yScale = (0.0 + to.getImage().rows) / from.getImage().rows;

  for (int i=0; i < from.getLandmarks().size(); i++) {
    double newx = from.getLandmarks()[i].x * xScale;
    double newy = from.getLandmarks()[i].y * yScale;
    scaledLandmarks[i] = cv::Point(newx, newy);
  }

  CThinPlateSpline tps(scaledLandmarks, to.getLandmarks());
  tps.warpImage(scaledFrom, warpedFrom);
  face newface(warpedFrom, to.getLandmarks());
  return newface;
}
