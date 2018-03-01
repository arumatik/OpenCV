#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
  Mat image(300, 300, CV_8UC3, Scalar(0, 0, 0));
  for (int i = 0; i < image.cols / 2 / 20 ; ++i)
    circle(image, Point(image.cols / 2, image.rows / 2), (i + 1) * 20,
           Scalar(255, 255, 255), 5);
  Mat dst;

  namedWindow("circle goal");
  imshow("circle goal", image);
  waitKey(0);
  
  Sobel(image, dst, -1, 1, 0, 3);
  imshow("circle goal", dst);
  waitKey(0);
  
  Sobel(image, dst, -1, 1, 0, 9);
  imshow("circle goal", dst);
  waitKey(0);
  
  Sobel(image, dst, -1, 1, 0, 13);
  imshow("circle goal", dst);
  waitKey(0);
  
  return 0;
}
