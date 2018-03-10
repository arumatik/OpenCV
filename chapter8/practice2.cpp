#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


size_t calc_max_distance(const vector<Point> & contour, const size_t idx,
                         double & max_dist) {
  Point start = contour[idx];
  size_t idx_at_max = -1;
  for (size_t i = 0; i < contour.size(); i++) {
    double dist
        = pow(pow(contour[i].x - start.x, 2) + pow(contour[i].y - start.y, 2),
              1. / 2);
    if (dist > max_dist) {
      max_dist = dist;
      idx_at_max = i;
    }
  }
  return idx_at_max;
}


int main() {
  Mat image = imread("../image/polygon.png");
  Mat gray, binary;
  vector<vector<Point> > contours;
  
  cvtColor(image, gray, COLOR_BGR2GRAY);
  threshold(gray, binary, 100, 255, THRESH_BINARY);
  findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

  vector<vector<Point> > maximum_points(contours.size());
  for (size_t i = 0; i < contours.size(); i++) {
    double max_dist = 0;
    vector<size_t> idx_at_max(2);
    for (size_t j = 0; j < contours[i].size(); j++) {
      size_t idx = calc_max_distance(contours[i], j, max_dist);
      if (static_cast<int>(idx) != -1) {
        idx_at_max[0] = j;
        idx_at_max[1] = idx;
      }
    }
    maximum_points[i].push_back(contours[i][idx_at_max[0]]);
    maximum_points[i].push_back(contours[i][idx_at_max[1]]);
  }

  for (size_t i = 0; i < contours.size(); i++) {
    line(image, maximum_points[i][0], maximum_points[i][1],
         Scalar(0, 0, 255), 3);
  }

  imshow("max dist", image);
  waitKey();
}
