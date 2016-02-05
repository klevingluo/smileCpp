#include "../src/face.h"
#include <iostream>

using namespace std;

int main() {

  face test1("./images/A.png", "./images/landmarks/A.txt");
  face test2("./images/obama.jpg", "./images/landmarks/obama.txt");
  face test3("./images/obama.jpg");
  face test4("./faces/MyD_039_1.jpg");

  // constructor test x 2
  // get feature test
  // get image test
  imshow("obama face", test2.getImage());
  waitKey(0);
  // get landmarks test
  
  // draw landmarks test
  imshow("landmark points model", test1.drawPoints());
  imshow("landmark points obama", test2.drawPoints());
  imshow("landmarks dlib face", test3.drawPoints());
  imshow("dlib2", test4.drawPoints());
  imshow("messed up?", test3.getFeature(face::MOUTH));
  imshow("ok?", test2.getFeature(face::MOUTH));


  imshow("warp", test4.warpTo(test3).drawPoints());
  waitKey(0);


  return 0;
}
