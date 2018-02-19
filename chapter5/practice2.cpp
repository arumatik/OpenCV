#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main() {
  Mat source = imread("../image/fruits.jpg");
  Mat g_filter;
  Mat image = Mat(source.rows, source.cols * 2, source.type());

  Rect roi1 = Rect(0, 0, source.cols, source.rows);
  Rect roi2 = Rect(source.cols, 0, source.cols, source.rows);
  source.copyTo(image(roi1));
  
  GaussianBlur(source, g_filter, Size(0, 0), 10);
  g_filter.copyTo(image(roi2));

  namedWindow("gaussian filter");

  while (1) {
    imshow("gaussian filter", image);
    if (waitKey(0) == 27) break;
  }

  return 0;
}
