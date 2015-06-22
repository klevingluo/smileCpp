#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ThinPlateSpline/CThinPlateSpline.h"
#include "./face.h"

using namespace cv;
using namespace std;

face process(face f);
face warpTo(face from, face to);
void testTransform();

int main () {
  testTransform();

  return(0);
}

void testTransform() {

  double scale = 7.0;
  face withMakeup("./images/A*.png", "./images/A*.txt");
  face WoMakeup("./images/A.png", "./images/A.txt");
  face input("./images/B.jpg", "./images/B.txt");

  Mat makeup = process(warpTo(withMakeup,input)).getImage();
  cout << "image 1 warped" << endl;
  Mat skin = process(warpTo(WoMakeup,input)).getImage();

  cout << "image 2 warped" << endl;

  Mat alpha(makeup.rows, makeup.cols, CV_8UC3);

  Mat labmakeup;
  Mat labskin;
  cvtColor(makeup, labmakeup, cv::COLOR_BGR2Lab);
  cvtColor(skin, labskin, cv::COLOR_BGR2Lab);
  absdiff(labmakeup, labskin, alpha);
  cv::cvtColor(alpha, alpha, cv::COLOR_BGR2GRAY);
  cv::cvtColor(alpha, alpha, cv::COLOR_GRAY2BGR);

  for(int i=0; i < alpha.rows; i++) {
    for(int j=0; j < alpha.cols; j++) {
      if(alpha.at<int>(Point(i, j)) < 80)
        alpha.at<int>(Point(i, j)) = 0;
    }
  }
  alpha *= scale;

  cv::GaussianBlur(alpha, alpha, Size(21,21), 0,0);
  
  cout << "alpha mask created" << endl;

  Mat base = input.getImage();

  Mat finalim(makeup.rows, makeup.cols, CV_8UC3);

  finalim = makeup.mul(alpha, 1.0/255) + base.mul(cv::Scalar::all(255) - alpha, 1.0/255);
  
  imshow("original", base);
  imshow("alpha", alpha);
  imshow("im1", makeup);
  imshow("im2", skin);
  imshow("makeup", makeup.mul(alpha, 1.0/255));
  imshow("finalim", finalim);
  imshow("example", withMakeup.getImage());

  waitKey(0);
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
