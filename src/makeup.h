#ifndef MAKEUP_LIBRARY
#define MAKEUP_LIBRARY
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "face.h"

using namespace std;
using namespace cv;

class makeup : public face{
  public:
    makeup(face example);

    makeup(face makeup, face model);

    face applyTo(face model);

  private:
    /**
     * transfers the makeup from local image on path example_image with 
     * facepp landmarks example_landmarks
     * to local image on path input_image with landmarks imput_landmarks
     * returns a cv::MAT
     */
    static Mat TransferMakeup(face input, face example);

    /**
     * transfers 
     */
    static Mat TransferMakeup(face input, face makeup, face model);
  
    /**
     * calculates the makeup application
     */
    static face getMask(face makeup, face skin);

    static face process(face f);
};
#endif
