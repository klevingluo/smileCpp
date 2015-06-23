#include "../src/face.h"
#include <iostream>

int main() {

  face test1("../images/A.png", "../images/landmarks/A.txt");
  face test2("../images/obama.jpg", "../images/landmarks/obama.txt");

  // constructor test x 2
  // get feature test
  // get image test
  imshow("obama face", test2.getImage());
  waitKey(0);
  // get landmarks test
  // get mask tes
  
  // draw landmarks test
  imshow("landmark points model", test1.drawPoints());
  imshow("landmark points obama", test2.drawPoints());
  waitKey(0);

  return 0;
}
