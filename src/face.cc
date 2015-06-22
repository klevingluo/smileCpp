#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <vector>
#include "face.h"

using namespace std;
using namespace cv;

// constants defined by facepp api
const vector<int> head = {1,2,3,4,5,6,7,8,9,0,18,17,16,15,14,13,12,11,10};
const vector<int> forehead = {11,10,83,84,85,85,86,87,88,89,90,1,2};
const vector<int> mouth = {37,40,41,38,44,43,46,52,51,54,48,49};
const vector<int> nose = {59,60,63,61,58,57,62,56,55};
const vector<int> leftbrow = {29,30,31,32,33,36,35,34};
const vector<int> rightbrow = {75,76,77,78,79,82,81,80};
const vector<int> lefteye = {21,22,19,23,25,28,26,27};
const vector<int> righteye = {67,68,65,69,71,74,72,73};

face::face(string imagePath, string landmarksPath) {
  image = imread(imagePath);
  std::vector<std::vector<double> > pts(2, std::vector<double>(83)); 
  std::string str;
  std::ifstream ifs(landmarksPath.c_str());
  if (ifs.is_open()) {
    for (int i=0; i<83; i++){
      getline(ifs, str);
      pts[0][i] = atof(str.c_str());
    }
    for (int i=0; i<83; i++) {
      getline(ifs, str);
      pts[1][i] = atof(str.c_str());
    }
  }
  ifs.close();
  vector<Point> points(91);
  for (int i=0; i<83; i++) {
    points[i] = Point(pts[0][i], pts[1][i]);
  }
  landmarks = points;
  generateForehead();
};

face::face(Mat i, vector<Point> lm) {
  image = i;
  landmarks = lm;
  generateForehead();
};

Mat face::getFeature(face::feature f) {
  vector<int> outline;
  switch(f) {
    case FOREHEAD:
      outline = forehead;
      break;
    case HEAD:
      outline = head;
      break;
    case MOUTH:
      outline = mouth;
      break;
    case NOSE:
      outline = nose;
      break;
    case LEFT_BROW:
      outline = leftbrow;
      break;
    case RIGHT_BROW:
      outline = rightbrow;
      break;
    case LEFT_EYE:
      outline = lefteye;
      break;
    case RIGHT_EYE:
      outline = righteye;
      break;
  }

  vector<Point> result(outline.size());
  for(int i=0 ;i<outline.size(); i++) {
    result[i] = landmarks[outline[i]];
  }

  Mat mask(image.rows, image.cols, CV_8UC1);
    for(int i=0; i<mask.cols; i++)
      for(int j=0; j<mask.rows; j++)
        mask.at<uchar>(Point(i,j)) = 0;
  
  cv::fillConvexPoly(mask, &result[0], result.size(), 255,8,0);
  return mask;
};

vector<Point> face::getLandmarks() {
  return landmarks;
};

Mat face::getImage() {
  return image.clone();
};

void face::generateForehead() {
  int rad = sqrt(pow(landmarks[10].x - landmarks[1].x,2) + 
                 pow(landmarks[10].y - landmarks[1].y,2))/2;

  Point center = Point((landmarks[10].x + landmarks[1].x)/2, 
                       (landmarks[10].y + landmarks[1].y)/2);
  for (int i=1; i<9; i++) {
    landmarks[82 + i] = Point(cos(M_PI/9 * i)*rad + center.x,
                         -0.9 * sin(M_PI/9 * i)*rad + center.y);
  }
};
