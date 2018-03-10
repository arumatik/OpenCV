#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


int main() {
  Mat image = imread("../image/bottle.jpg");
  Mat gray, binary;
  vector<vector<Point> > contours;
  
  cvtColor(image, gray, COLOR_BGR2GRAY);
  threshold(gray, binary, 240, 255, THRESH_BINARY_INV);
  
  findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
  drawContours(image, contours, -1, Scalar(0, 0, 255));
  imshow("model", image);

  Mat target = imread("../image/bottle2.jpg");\
  Mat target_gray, target_binary;
  vector<vector<Point> > target_contours;
  
  cvtColor(target, target_gray, COLOR_BGR2GRAY);
  threshold(target_gray, target_binary, 240, 255, THRESH_BINARY_INV);
  findContours(target_binary, target_contours,
               RETR_EXTERNAL, CHAIN_APPROX_NONE);
  drawContours(target, target_contours, -1, Scalar(0, 0, 255));
  imshow("target", target);

  double match = matchShapes(binary, target_binary, CONTOURS_MATCH_I1, 0);
  cout << match << endl;
  
  waitKey();
}
