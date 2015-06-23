#include <string>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "makeup.h"
#include "ThinPlateSpline/CThinPlateSpline.h"
#include "face.h"

double scale = 7.0;

cv::Mat makeup::TransferMakeup(Mat input_image, vector<Point> input_landmarks, 
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
  face lay = face(example, input_landmarks);

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

  return finalMat;
}

face makeup::getMask(face makeup, face skin) {

  Mat labmakeup;
  Mat labskin;
  Mat alpha;

  cvtColor(makeup.getImage(), labmakeup, cv::COLOR_BGR2Lab);
  cvtColor(skin.getImage(), labskin, cv::COLOR_BGR2Lab);
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

  Mat finalim(labskin.rows, labskin.cols, CV_8UC3);

  finalim = makeup.getImage().mul(alpha, 1.0/255) + makeup.getImage().mul(cv::Scalar::all(255) - alpha, 1.0/255);
 
  return face(finalim, makeup.getLandmarks());
}
                      
face applyMakeup(face model, face mask); 

void testTransform() {

  double scale = 7.0;
  face withMakeup("./images/A*.png", "./images/A*.txt");
  face WoMakeup("./images/A.png", "./images/A.txt");
  face input("./images/B.jpg", "./images/B.txt");

  Mat makeup = makeup::process(makeup::warpTo(withMakeup,input)).getImage();
  cout << "image 1 warped" << endl;
  Mat skin = makeup::process(makeup::warpTo(WoMakeup,input)).getImage();

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

face makeup::process(face f) {
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
