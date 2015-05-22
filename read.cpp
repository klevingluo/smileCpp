#include <string>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

double landmarks [2][83];

int main() {
  string str;
  ifstream ifs("obama.txt");
  if (ifs.is_open()) {
    for (int i=0; i<83; i++){
      getline(ifs, str);
      landmarks[0][i] = atof(str.c_str());
    }
    for (int i=0; i<83; i++) {
      getline(ifs, str);
      landmarks[1][i] = atof(str.c_str());
    }
  }

  cout << landmarks[0][82] << " : " << landmarks[1][82] << endl;

  Mat image;
  Mat lab_image;


  image = imread("obama.jpg", CV_LOAD_IMAGE_COLOR);

  cv::cvtColor(image, lab_image, 3);

  namedWindow( "Display window", WINDOW_AUTOSIZE );
  namedWindow( "lab_image", WINDOW_AUTOSIZE);

  imshow("lab_image", lab_image);
  imshow( "Display window", image);

  waitKey(0); 
  return 0;
}
