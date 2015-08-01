#ifndef MAKEUP_EXAMPLE
#define MAKEUP_EXAMPLE

#include "makeup.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <fstream>

class exampleMakeup : public makeup, public face {
  public:
    exampleMakeup(face);
                      
    face applyTo(face);
};

#endif
