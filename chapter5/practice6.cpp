#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
  Mat image = imread("../image/fruits.jpg");
  Mat gray, tophat;
  
  cvtColor(image, gray, COLOR_BGR2GRAY);
  morphologyEx(gray, tophat, MORPH_TOPHAT, Mat());

  Mat output(image.rows, image.cols * 2, CV_8UC1);

  gray.copyTo(output(Rect(0, 0, image.cols, image.rows)), tophat);
  tophat.copyTo(output(Rect(image.cols, 0, image.cols, image.rows)));

  namedWindow("tophat");
  imshow("tophat", output);

  waitKey(0);

  
  
  return 0;
}
