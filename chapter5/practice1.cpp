#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
  Mat image = imread("../image/texture.jpg");
  Mat g5_filter, g11_filter, tmp;

  image = Mat(100, 100, CV_8UC1, Scalar(255));
  image.ptr<unsigned char>(image.rows / 2)[image.cols / 2] = 0;
  image.ptr<unsigned char>(image.rows / 2 + 1)[image.cols / 2 + 1] = 0;
  image.ptr<unsigned char>(image.rows / 2 + 1)[image.cols / 2] = 0;
  image.ptr<unsigned char>(image.rows / 2)[image.cols / 2 + 1] = 0;
  
  GaussianBlur(image, tmp, Size(5, 5), 0);
  GaussianBlur(tmp, g5_filter, Size(5, 5), 0);
  GaussianBlur(image, g11_filter, Size(11, 11), 0);
  namedWindow("source");
  namedWindow("5 x 5 filter");
  namedWindow("11 x 11 filter");

  while (1) {
    imshow("source", image);
    imshow("5 x 5 filter", g5_filter);
    imshow("11 x 11 filter", g11_filter);
    if (waitKey(0) == 27) break;
  }

  return 0;
}
