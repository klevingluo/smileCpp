#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../src/makeup.h"
#include "../src/face.h"
#include "../src/exampleMakeup.h"

using namespace cv;
using namespace std;

int main() {

  face test1("./images/A*.png", "./images/landmarks/A*.txt");
  face test2("./images/obama.jpg", "./images/landmarks/obama.txt");

  imshow("example", test1.getImage());
  imshow("input", test2.getImage());

  exampleMakeup mkp(test1); 

  face makeup = mkp.applyTo(test2);

  imshow("output", makeup.getImage());

  imwrite("../../output.jpg", makeup.getImage());

  waitKey(0);
    
  return 0;
}
