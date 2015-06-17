#include <string>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace std;
using namespace cv;

class layers {

  private:
    Point landmarks_[91];
    Mat face_;
    //defined shapes for the facial features, according to the 
    //return definitions of the face++ api
    const vector<int> head = {1,2,3,4,5,6,7,8,9,0,18,17,16,15,14,13,12,11,10};
    const vector<int> forehead = {11,10,83,84,85,85,86,87,88,89,90,1,2};
    const vector<int> mouth = {37,40,41,38,44,43,46,52,51,54,48,49};
    const vector<int> nose = {59,60,63,61,58,57,62,56,55};
    const vector<int> leftbrow = {29,30,31,32,33,36,35,34};
    const vector<int> rightbrow = {75,76,77,78,79,82,81,80};
    const vector<int> lefteye = {21,22,19,23,25,28,26,27};
    const vector<int> righteye = {67,68,65,69,71,74,72,73};

  public:
    layers(Mat face, vector<Point> landmarks) {
      for(int i=0; i < 83; i++) {
        landmarks_[i] = landmarks[i];
      }
      int rad = sqrt(pow(landmarks[10].x - landmarks[1].x,2) + 
                     pow(landmarks[10].y - landmarks[1].y,2))/2;
      Point center = Point((landmarks[10].x + landmarks[1].x)/2, 
                           (landmarks[10].y + landmarks[1].y)/2);

      for (int i=1; i<9; i++) {
        landmarks_[82+i] = Point(cos(M_PI/9 * i)*rad + center.x,
                                 -0.7 * sin(M_PI/9 * i)*rad + center.y);
      }
      face_ = face.clone();
    };

    Mat getLightness() {
      Mat lab;  
      cv::cvtColor(face_, lab, cv::COLOR_BGR2Lab);
          
      Mat *labChannels = new Mat[lab.channels()];
      cv::split(lab, labChannels);
      return labChannels[0];
    }

    Mat getShapes() {
      Mat mask = Mat(face_.rows, face_.cols, CV_8UC1);
      for(int i=0; i<mask.cols; i++)
        for(int j=0; j<mask.rows; j++)
          mask.at<uchar>(Point(i,j)) = 0;

      int wt = face_.cols / 40;
      int ht = face_.rows / 40;
      if(wt % 2 != 1) {
        wt += 1;
      }
      if(ht % 2 != 1) {
        ht += 1;
      }

      fillConvexPoly(mask, &getPoly(forehead)[0], forehead.size(), 255,8,0);
      cv::GaussianBlur(mask, mask, Size(wt,ht*11), 0, 0);

      fillConvexPoly(mask, &getPoly(head)[0], head.size(), 255,8,0);
      fillConvexPoly(mask, &getPoly(mouth)[0], mouth.size(), 0,8,0);
      fillConvexPoly(mask, &getPoly(leftbrow)[0], leftbrow.size(), 75,8,0);     
      fillConvexPoly(mask, &getPoly(rightbrow)[0], rightbrow.size(), 75,8,0);
      //cv::GaussianBlur(mask, mask, Size(wt*3,ht*3), 0,0);

      fillConvexPoly(mask, &getPoly(righteye)[0], righteye.size(), 0,8,0);
      fillConvexPoly(mask, &getPoly(lefteye)[0], lefteye.size(), 0,8,0);
      //cv::GaussianBlur(mask, mask, Size(wt,ht), 0,0);

      return mask;
    }

    vector<Point> getPoly(vector<int> v) {
      vector<Point> result = vector<Point>(v.size());
      for(int i=0 ;i<v.size(); i++) {
        result[i] = landmarks_[v[i]];
      }
      return result;
    }
};
