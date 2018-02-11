#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main() {
  Mat img = Mat(300, 300, CV_8UC3, Scalar(0, 0, 0));
  circle(img, Point(100,100), 100, Scalar(0, 0, 255));
  int topleft = (300 * 5 + 20 - 1) * 3 + 1;
  int point;
  for (int i = 0; i < 20; i++) {
    point = topleft + i * 3;
    img.data[point] = 255;
  }
  for (int i = 0; i < 15; i++) {
    img.data[point + i * 3 * 300] = 255;
  }
  for (int i = 0; i < 15; i++) {
    point = topleft + i * 3* 300;
    img.data[point] = 255;
  }
  for (int i = 0; i < 20; i++) {
    img.data[point + i * 3] = 255;
  }
  
  namedWindow("practice", CV_WINDOW_AUTOSIZE);
  imshow("practice", img);

  waitKey(0);
  img.release();
  destroyAllWindows();  
}
