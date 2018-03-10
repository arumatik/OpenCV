#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


double CURVELENGTH = 125.6;


void make_circle_image() {
  Mat image(200, 200, CV_8UC3, Scalar::all(0));
  circle(image, Point(image.cols / 2, image.rows / 2), 20, Scalar::all(255));
  imwrite("../image/circle.jpg", image);
}


void calc_contour_length(Mat image, const vector<vector<Point> > & contours) {
  for (size_t i =0; i < contours.size(); i++) {
    double length = arcLength(contours[i], true);
    cout << "contour length : " << length << endl;
    for (size_t j = 0; j < contours[i].size(); j++) {
      Point point = contours[i][j];
      image.at<Vec3b>(point.y, point.x) = Vec3b(0, 0, 255);
    }
    imshow("contour", image);
    waitKey();
  }

}


void draw_approx_curve(Mat image, const double scale,
                       const vector<vector<Point> > & contours) {
  for (size_t i = 0; i < contours.size(); i++) {
    vector<Point> approx;
    approxPolyDP(contours[i], approx, CURVELENGTH * scale, true);
    for (size_t j = 0; j < approx.size(); j++) {
      line(image, approx[j], approx[(j + 1) % approx.size()],
           Scalar(0, 0, 255));
    }
    double length = arcLength(approx, true);
    cout << "approx curve lengte : " << length << endl;
    imshow("approx curve", image);
    waitKey();
  }
}


int main() {
  //make_circle_image();
  Mat image = imread("../image/circle.jpg");
  Mat gray, binary;
  vector<vector<Point> > contours;
  
  cvtColor(image, gray, COLOR_BGR2GRAY);
  threshold(gray, binary, 100, 255, THRESH_BINARY);
  
  findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
  //calc_contour_length(image, contours);

  vector<double> scale {0.9, 0.66, 0.33, 0.1};

  for (auto itr = scale.begin(); itr < scale.end(); itr++){
    draw_approx_curve(image, *itr, contours);
  }
}
