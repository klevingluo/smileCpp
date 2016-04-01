#include <foundation.h>
#include <iostream>
#include <opencv2/photo/photo.hpp>
#include <opencv2/core/core.hpp>
#include <softLightBlend.h>

using namespace cv;
using namespace std;

face getMask(face f);
Mat blur(Mat m);
Scalar color;

foundation::foundation(int R, int B, int G) {
  color = Scalar(B, G, R);
}

face foundation::applyTo(face model, double weight) {

  Mat pattern = model.getImage().clone();
  pattern.setTo(color);

  Mat foundation = softLightBlend(pattern, model.getImage());

  imshow("face", model.getImage());
  imshow("foundation color", pattern);
  imshow("foundation", foundation);

  Mat alpha = getMask(model).getImage();
  cv::cvtColor(alpha, alpha, cv::COLOR_BGR2GRAY);
  alpha = blur(alpha);
  alpha *= weight;
  imshow("mask", alpha);

  Mat *makeup = new Mat[3];
  Mat *image = new Mat[3];
  Mat *finalim = new Mat[3];

  cv::split(model.getImage(), image);
  cv::split(foundation, makeup);
  cv::split(foundation, finalim);

  for (int i=0; i<3; i++) {
    finalim[i] = makeup[i].mul(alpha) * 1.0/255 +
                 image[i].mul(255 - alpha) * 1.0/255;
  }

  Mat result;
  cv::merge(finalim, 3, result);
  return face(result, model.getLandmarks());
};

Mat blur(Mat m) {
  m*= 20;
  
  cv::GaussianBlur(m, m, Size(51,51), 0,0);
  return m;
}

face getMask(face f) {
  Mat mask = f.getImage();

  Mat removals = 255 - 
                 f.getFeature(face::FOREHEAD) -
                 f.getFeature(face::HEAD) + 
                 f.getFeature(face::LEFT_EYE) + 
                 f.getFeature(face::RIGHT_EYE) +
                 f.getFeature(face::LEFT_BROW) + 
                 f.getFeature(face::RIGHT_BROW) +
                 f.getFeature(face::MOUTH);

  vector<Mat> layers;
  cv::split(mask, layers);

  for(int i=0; i<layers.size(); i++) {
    layers[i] = layers[i] - removals;
  }

  cv::merge(layers, mask);

  face m(mask, f.getLandmarks());
  return m;
}
