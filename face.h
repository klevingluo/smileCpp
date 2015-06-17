#ifndef MAKEUP_FACE
#define MAKEUP_FACE
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

class face {

  public:
    enum feature {HEAD, FOREHEAD, MOUTH, NOSE, LEFT_BROW, RIGHT_BROW,
                  LEFT_EYE, RIGHT_EYE};
    face(string imagePath, string landmarksPath);
    face(Mat image, vector<Point> landmarks);
    Mat getFeature(feature f);
    Mat getImage();
    vector<Point> getLandmarks();
};

#endif
