#include <softLightBlend.h>
#include <iostream>

using namespace cv;
using namespace std;

int main() {

  Mat img1 = imread("./images/top-layer.jpg");
  Mat img2 = imread("./images/bottom-layer.jpg");

  imshow("top layer", img1);
  imshow("bottom layer", img2);

  // images must be of the same size
  Size size = img1.size();
  cv::resize(img2, img2, size);

  Mat blend = softLightBlend(img1, img2);

  cv::imshow("blend", blend);
  cv::waitKey(0);
  return 0;
}
