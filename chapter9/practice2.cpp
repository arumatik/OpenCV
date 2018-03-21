#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;


int main() {
  Mat image = imread("../image/fruits_inpaint.jpg");
  Mat mask(image.size(), CV_8UC1, Scalar::all(0));
  Mat dst;
  double radius = 10.;

  mask.forEach<uchar>([&image] (uchar & p, const int * position) -> void {
      Vec3b pix = image.at<Vec3b>(position[0], position[1]);
      if (pix[0] == 0 || pix[1] == 0 || pix[2] == 0) p = 255;
    });
  morphologyEx(mask, mask, MORPH_CLOSE, Mat());
  
  inpaint(image, mask, dst, radius, INPAINT_TELEA);

  imshow("source", image);
  imshow("mask", mask);
  imshow("result", dst);

  waitKey(0);
}
