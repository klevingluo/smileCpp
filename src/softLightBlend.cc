#include <opencv2/core/core.hpp>
#include <math.h>
#include <softLightBlend.h>

using namespace cv;
using namespace std;

/**
 * soft light blending
 *
 * note that images must be of the same size and also be of CV_8U3C format
 *
 * img1 is the upper layer
 * img2 is the lower layer
 */
Mat softLightBlend(Mat img1, Mat img2) {

  Mat *upper = new Mat[3]; 
  Mat *lower = new Mat[3]; 
  Mat *result = new Mat[3];

  cv::split(img1, upper);
  cv::split(img2, lower);

  for (int i=0; i<3; i++) {
    result[i] = lower[1].clone();
  }

  int rows = img1.rows;
  int cols = img1.cols;

  for (int x=0; x<rows; x++) {
    for (int y=0; y<cols; y++) {
      for (int i=0; i<3; i++) {
        double upperPixel = upper[i].at<unsigned char>(x,y) / 255.0;
        double lowerPixel = lower[i].at<unsigned char>(x,y) / 255.0;

        if(upperPixel < 0.5) {
          result[i].at<unsigned char>(x,y) = (unsigned char)((2*upperPixel*lowerPixel + 
                                            lowerPixel*lowerPixel * 
                                            (1 - 2*upperPixel))*255.0);
        } else {
          result[i].at<unsigned char>(x,y) = (unsigned char)((2*lowerPixel*(1-upperPixel) + 
                                            sqrt(lowerPixel) * 
                                            (2*upperPixel - 1))*255.0);
        }
      }
    }
  }

Mat output;
merge(result,3,output);
return output; 
}
