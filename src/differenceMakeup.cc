#include "differenceMakeup.h"
#include <iostream>
#include <opencv2/photo/photo.hpp>

face process(face f);
Mat filter(Mat m);

double scale = 20.0;

differenceMakeup::differenceMakeup(face model, face makeup)
  :face(makeup.getImage(), makeup.getLandmarks()) {

    Mat makeupLayer = process(makeup).getImage();
    Mat skinLayer = (process(warpTo(model,makeup))).getImage();

    Mat mask(makeup.getImage().rows, makeup.getImage().cols, CV_8UC3);

    Mat labmakeup;
    Mat labskin;

    cvtColor(makeupLayer, labmakeup, cv::COLOR_BGR2Lab);
    cvtColor(skinLayer, labskin, cv::COLOR_BGR2Lab);

    absdiff(labmakeup, labskin, mask);
    
    cv::cvtColor(mask, mask, cv::COLOR_BGR2GRAY);

    mask = filter(mask);

    imshow("makeup", makeupLayer);
    imshow("model", skinLayer);

    vector<Mat> colors(3);
    cv::split(makeup.getImage(), colors);

    vector<Mat> masked(4);
    
    masked[0] = colors[0];
    masked[1] = colors[1];
    masked[2] = colors[2];
    masked[3] = mask;

    cv::merge(masked, image);
  }

face differenceMakeup::applyTo(face model, double weight) {

  face newface = warpTo(model);
  image = newface.getImage();
  landmarks = model.getLandmarks();

  vector<Mat> layers(4);
  split(image, layers);

  vector<Mat> colors(3);
  colors[0] = layers[0];
  colors[1] = layers[1];
  colors[2] = layers[2];

  Mat makeup;
  merge(colors, makeup);

  Mat alpha;
  cv::cvtColor(layers[3], alpha, cv::COLOR_GRAY2BGR);

  alpha *= weight;

  Mat finalim = makeup.mul(alpha, 1.0/255) + 
                model.getImage().mul(cv::Scalar::all(255) - alpha, 1.0/255);

  return face(finalim, model.getLandmarks());
};

Mat filter(Mat m) {
  m*= scale;
  
  cv::GaussianBlur(m, m, Size(51,51), 0,0);
  return m;
}

face process(face f) {
  Mat mask = f.getImage();

  Mat removals = 255 - 
                 f.getFeature(face::FOREHEAD) -
                 f.getFeature(face::HEAD) + 
                 f.getFeature(face::LEFT_EYE) + 
                 f.getFeature(face::RIGHT_EYE) +
                 f.getFeature(face::LEFT_BROW) + 
                 f.getFeature(face::RIGHT_BROW) +
                 f.getFeature(face::NOSE);

  vector<Mat> layers;
  cv::split(mask, layers);

  for(int i=0; i<layers.size(); i++) {
    layers[i] = layers[i] - removals;
  }

  cv::merge(layers, mask);

  face m(mask, f.getLandmarks());
  return m;
}
