#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char ** argv) {
  Mat image = imread("../image/fruits.jpg");
  vector<Mat> edges(3);
  double high = 200.; 

  Canny(image, edges[0], high, high / 1.5);
  Canny(image, edges[1], high, high / 2.75);
  Canny(image, edges[2], high, high / 4.);

  namedWindow("canny");
  for (int i = 0; i < 3; ++i) {
    imshow("canny", edges[i]);
    waitKey(0);
  }

  return 0;
}
