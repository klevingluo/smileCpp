#include <face.h>
#include <iostream>

using namespace std;

int main() {

  face test1("./images/A.png", "./images/landmarks/A.txt");
  face test2("./images/obama.jpg", "./images/landmarks/obama.txt");

  waitKey(0);


  return 0;
}
