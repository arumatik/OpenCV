#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char ** argv) {
  Mat image;
  if (argc > 1) {
    image = imread(argv[1]);
  }
  else {
    image = imread("../image/fruits.jpg");
  }
  Mat kernel = (Mat_<double>(3, 3) << 1. / 16, 2. /16, 1. / 16, 2. / 16,
                4. / 16, 2. / 16, 1. / 16, 2. / 16 , 1. /16);
  Mat kernel1 = (Mat_<double>(1, 3) << 1. / 4, 2. / 4, 1. / 4);
  Mat kernel2 = (Mat_<double>(3, 1) << 1. / 4, 2. / 4, 1. / 4);
  Mat dst, dst1, dst2;

  filter2D(image, dst, -1, kernel);
  filter2D(image, dst1, -1, kernel1, Point(1, 0));
  filter2D(dst1, dst2, -1, kernel2, Point(0, 1));

  namedWindow("gaussian kernel");
  imshow("gaussian kernel", image);
  waitKey(0);
  
  imshow("gaussian kernel", dst);
  waitKey(0);

  imshow("gaussian kernel", dst2);
  waitKey(0);

  return 0;
}
