#include <vector>
#include <string>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../src/makeup.h"

using namespace cv;
using namespace std;

vector<Point> GetPoints(string filename);

int main() {

  std::vector<cv::Point> inputmarks = GetPoints("B.txt");
  std::vector<cv::Point> examplemarks = GetPoints("A*.txt");

  cv::Mat input = cv::imread("./images/B.jpg");
  cv::Mat example = cv::imread("./images/A*.png");

  makeup eg = makeup();
  Mat sho = eg.TransferMakeup(input, inputmarks, example, examplemarks);

  imshow("transferred makeup", sho);
  imshow("example", example);
  imshow("original", input);

  waitKey(0);
  return 0;
}
  /**
   * function that reads the landmark points from a file generated for the purpose
   *
   * filename - the filename of the landmarks file, with extension (.txt in the examples)
   */
  std::vector<cv::Point> GetPoints(std::string filename) {
    std::vector<std::vector<double> > pts(2, std::vector<double>(83));
    std::string str;
    std::string path = "images/" + filename;
    std::ifstream ifs(path.c_str());
    if (ifs.is_open()) {
      for (int i=0; i<83; i++){
        getline(ifs, str);
        pts[0][i] = atof(str.c_str());
      }
      for (int i=0; i<83; i++) {
        getline(ifs, str);
        pts[1][i] = atof(str.c_str());
      }
    }
    ifs.close();
    std::vector<cv::Point> points(83);
    for (int i=0; i<83; i++) {
      points[i] = cv::Point(pts[0][i], pts[1][i]);
    }
    return points;
  }
