#include <string>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "./ThinPlateSpline/CThinPlateSpline.cpp"

using namespace std;
using namespace cv;
/**
 * function that reads the landmark points from a file generated for the purpose
 *
 * filename - the filename of the landmarks file, with extension (.txt in the examples)
 */
vector<Point> getPoints(string filename) {
  vector<vector<double> > pts(2, vector<double>(83));
  string str;
  string path = "images/" + filename;
  ifstream ifs(path.c_str());
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
  vector<Point> points(83);
  for (int i=0; i<83; i++) {
    points[i] = Point(pts[0][i], pts[1][i]);
  }
  return points;
}

int main() {
  string str;
  
  // reads the landmarks from the file
  vector<Point> landmarks = getPoints("example4.txt");
  vector<Point> destinations = getPoints("input.txt");

  CThinPlateSpline tps(landmarks, destinations);

  Mat exampleRaw = imread("./images/example4.png");
  Mat input = imread("./images/input.jpg");
  Mat example;

  // warps the image
  tps.warpImage(exampleRaw, example, INTER_CUBIC, BACK_WARP);
  Rect crop(0,0, min(example.cols, input.cols), min(example.rows, input.rows));

  example = example(crop);
  input = input(crop);

  imshow("example", example);
  imshow("INPUT", input);

  Mat deriv;
  cv::Scharr(input, deriv,  CV_16S, 0, 1, 7, -75, cv::BORDER_DEFAULT);
  imshow("derivative", deriv);

  Mat deriv2;
  cv::Scharr(input, deriv2,  CV_16S, 1, 0, 7, -75, cv::BORDER_DEFAULT);
  imshow("derivative2", deriv2);
  
  Mat lab;
  cv::cvtColor(example, lab, cv::COLOR_BGR2Lab);
  
  Mat *labChannels = new Mat[lab.channels()];
  cv::split(lab, labChannels);

  imshow("greyscale", labChannels[0]);

  Mat a(lab.rows, lab.cols, CV_8U);
  
  Mat empty = Mat::zeros(lab.rows, lab.cols, CV_8U);
  Mat ab[] = {empty, a, a};
  int from_to[] = {1,1,2,2};
  cv::mixChannels(labChannels, 3, ab, 3, from_to, 2);

  Mat abimage;
  cv::merge(ab, 3, abimage);

  cvtColor(abimage, abimage, cv::COLOR_Lab2BGR);

  imshow("ab layer", abimage);


  Mat inputLab;
  cv::cvtColor(input, inputLab, cv::COLOR_BGR2Lab);
  
  Mat *InputLabChannels = new Mat[inputLab.channels()];
  cv::split(inputLab, InputLabChannels);


  Mat finalL = InputLabChannels[0].clone();
  Mat finalA;
  cv::addWeighted(InputLabChannels[1], 0.2, labChannels[1], 0.8, 0, finalA);
  Mat finalB;
  cv::addWeighted(InputLabChannels[2], 0.2, labChannels[2], 0.8, 0, finalB);

  Mat finalimage[] = {finalL.clone(), finalA.clone(), finalB.clone()};

  Mat finalMat;
  cv::merge(finalimage, 3, finalMat);

  cvtColor(finalMat, finalMat, cv::COLOR_Lab2BGR);

  imshow("pre-result", finalMat);

  waitKey(0); 
  return 0;
}
