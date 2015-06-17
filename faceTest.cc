#include "face.h"
#include <iostream>

int main() {

  face testface("./images/A.png", "./images/A.txt");


  imshow("nose", testface.getFeature(face::NOSE));
  
  waitKey(0);
  return 0;
}
