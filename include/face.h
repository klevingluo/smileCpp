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

    /**
     * defines the valid features for selection
     */
    enum feature {HEAD, FOREHEAD, MOUTH, NOSE, 
                  LEFT_BROW, RIGHT_BROW,
                  LEFT_EYE, RIGHT_EYE};

    /**
     * creates a face from the image specified and the landmarks file
     * specified
     */
    face(string, string);

    /**
     * creates a face from a mat and a list of landmarks
     */
    face(Mat, vector<Point>);

    /**
     * gets a mask of the feature f
     */
    Mat getFeature(feature);

    /**
     * gets the image of the face
     */
    Mat getImage();

    /**
     * gets the list of landmarks of the face
     */
    vector<Point> getLandmarks();

    /**
     * gets a mask of common makeup areas, for the 1 example algorithm
     */
    Mat getMask();

    /**
     * draws the landmark points of this face
     */
    Mat drawPoints();

    /**
     * warps this face to the geometry of another face
     */
    face warpTo(face);

    /**
     * warps one face to the goemetry of another face
     */
    static face warpTo(face, face);

  protected:
    Mat image;
    vector<Point> landmarks;
    // face++ api specific, used to generate a forehead for masking
    void generateForehead();
};
#endif
