#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
  Mat image = imread("../image/fruits.jpg");
  Mat kernel(3, 3, CV_32FC1, Scalar(0));
  Mat dst1, dst2;

  kernel.at<float>(0, 1) = 0.17;
  kernel.at<float>(0, 2) = 0.17;
  kernel.at<float>(1, 0) = 0.06;
  kernel.at<float>(1, 1) = 0.22;
  kernel.at<float>(1, 2) = 0.06;
  kernel.at<float>(2, 0) = 0.17;
  kernel.at<float>(2, 1) = 0.17;
  
  Canny(image, dst1, 150, 50);
  //HoughLines();
  
  namedWindow("test");
  imshow("test", dst1);
  waitKey(0);
  
  filter2D(dst1, dst1, -1, kernel);
  threshold(dst1, dst1, 150, 255, THRESH_BINARY);

  namedWindow("test");
  imshow("test", dst1);
  waitKey(0);

  return 0;
}
