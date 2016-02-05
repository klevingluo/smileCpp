#include "../src/differenceMakeup.h"
#include <iostream>

using namespace cv;
using namespace std;


int main() {

  face input("./images/B.jpg", "./images/landmarks/B.txt");
  face nomakeup("./images/A.png", "./images/landmarks/A.txt");
  face makeup("./images/A*.png", "./images/landmarks/A*.txt");

  differenceMakeup mk(nomakeup, makeup);

  std::cout << "applying makeup" << endl;
  face newface = mk.applyTo(input);

  cv::imshow("makeup",newface.getImage());

  cv::waitKey(0);
  return 0;
}
