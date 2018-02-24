#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main () {
  Mat random_img(300, 300, CV_8UC3);
  randu(random_img, 0, 3);
  //imwrite("../image/random_image.jpg", random_img);

  namedWindow("random image");
  random_img = imread("../image/random_image.jpg");
  imshow("random image", random_img);
  waitKey(0);

  Mat b_filter;
  bilateralFilter(random_img, b_filter, -1, 10, 5);

  imshow("random image", b_filter);
  waitKey(0);

  return 0;
}
