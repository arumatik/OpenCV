#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
  Mat src1 = imread("../image/home1.jpg");
  Mat src2 = imread("../image/home2.jpg");

  while (src1.rows > 1000 || src1.cols > 1000){
    pyrDown(src1, src1);
    pyrDown(src2, src2);
  }

  Mat image1 = Mat(src1.rows, src1.cols * 3, CV_8UC3);
  Mat image2 = Mat(src1.rows, src1.cols * 2, CV_8UC3);
  Rect roi1(0, 0, src1.cols, src1.rows);
  Rect roi2(src1.cols, 0, src1.cols, src1.rows);
  Rect roi3(src1.cols * 2, 0, src1.cols, src1.rows);

  Mat diff12, cleandiff, dirtydiff;
  
  absdiff(src1, src2, diff12);
  src1.copyTo(image1(roi1));
  src2.copyTo(image1(roi2));
  diff12.copyTo(image1(roi3));

  
  erode(diff12, cleandiff, Mat());
  dilate(cleandiff, cleandiff, Mat());
  dilate(diff12, dirtydiff, Mat());
  erode(dirtydiff, dirtydiff, Mat());
  cleandiff.copyTo(image2(roi1));
  dirtydiff.copyTo(image2(roi2));
  
  namedWindow("difference");
  namedWindow("dirty vs clean");

  while (1) {
    imshow("difference", image1);
    imshow("dirty vs clean", image2);
    if (waitKey(0) == 27) break;
  }

  return 0;
}
