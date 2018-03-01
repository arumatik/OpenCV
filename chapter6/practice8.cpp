#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
  Mat image(300, 300, CV_8UC3, Scalar(0, 0, 0));
  Mat dst;
  Point tl(50, 50), br(250, 250);
  rectangle(image, tl, br, Scalar(255, 255, 255));
  
  logPolar(image, dst, Point(tl.x - 3, tl.y - 3), 20, INTER_LINEAR);

  namedWindow("logpolar");
  imshow("logpolar", image);
  waitKey(0);
  imshow("logpolar", dst);
  waitKey(0);
  
  return 0;
}
