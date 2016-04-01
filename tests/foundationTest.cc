#include <opencv2/core/core.hpp>
#include <face.h>
#include <foundation.h>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
  face input("./images/B.jpg", "./images/landmarks/B.txt");

  foundation fnd(243, 220, 204);

  face newface = fnd.applyTo(input);

  cv::imshow("makeup",newface.getImage());

  cv::waitKey(0);
  return 0;
}
