#ifndef MAKEUP_INTERFACE
#define MAKEUP_INTERFACE
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "face.h"

using namespace std;
using namespace cv;

class makeup {
  public:
    virtual face applyTo(face) =0;
};
#endif
