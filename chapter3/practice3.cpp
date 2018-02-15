#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main() {
  int PIXEL = 210;
  Mat img = Mat(PIXEL, PIXEL, CV_8UC3, Scalar(0, 0, 0));
  
  for (int i = 0; i < PIXEL / 2; i += 20) {
    rectangle(img, Point(i, i), Point(PIXEL - i, PIXEL - i), Scalar(i, i, i),
              CV_FILLED);
  }
  
  namedWindow("practice", CV_WINDOW_AUTOSIZE);
  imshow("practice", img);

  waitKey(0);
  img.release();
  destroyAllWindows();  
}
