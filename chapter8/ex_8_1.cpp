#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat image, gray;
int thresh = 100;
vector<Mat> storage;

void on_trackbar(int, void*) {
  if (storage.empty())
    gray = Mat(image.size(), CV_8UC1);
  else
    storage.clear();

  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  cvtColor(image, gray, COLOR_BGR2GRAY);
  threshold(gray, gray, thresh, 255, THRESH_BINARY);
  imshow("binary", gray);
  findContours(gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
  gray = 0;

  if (!contours.empty()) {
    Scalar color(rand() & 255, rand() & 255, rand() & 255);
    drawContours(gray, contours, -1, color, 3, LINE_8, hierarchy, 1);
  }
  imshow("contour", gray);
}

int main(int argc, char ** argv) {
  image = imread(argc > 1 ? argv[1] : "../image/random_image.jpg");
  namedWindow("contour");
  createTrackbar("thresh", "contour", &thresh, 255, on_trackbar);
  imshow("source", image);
  on_trackbar(thresh, 0);
  waitKey();

  return 0;
}
