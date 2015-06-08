#ifndef MAKEUP_TRANSFER
#define MAKEUP_TRANSFER
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "./makeup.cc"

using namespace std;
using namespace cv;

/**
 * transfers the makeup from local image on path example_image with facepp landmarks example_landmarks
 * to local image on path input_image with landmarks imput_landmarks, returns a cv::MAT
 */
Mat TransferMakeup(String input_image, String input_landmarks,
                   String example_image, String example_landmarks);

/**
 * a demo, soon to be removed
 */
void Process();
  
#endif
