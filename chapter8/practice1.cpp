#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;


void detect_corner(Mat & image, const Mat & binary) {
  Mat curvature;
  
  cornerMinEigenVal(binary, curvature, 3, 3);
  normalize(curvature, curvature, 1, 0, NORM_MINMAX);
  threshold(curvature, curvature, 0.5, 1, THRESH_BINARY);

  for (int j = 0; j < curvature.rows; j++) {
    float * pixels = curvature.ptr<float>(j);
    for (int i = 0; i < curvature.cols; i++) {
      float pixel = pixels[i];
      if (pixel > 0.5)
        circle(image, Point(i, j), 5, Scalar(0, 0, 255));
    }
  }
  
  imshow("corner", image);
  waitKey();
}

void draw_contour(Mat & image, const Mat & binary) {
  vector<vector<Point> > contours;
  findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  for (size_t i = 0; i < contours.size(); i++) {
    size_t count = contours[i].size();
    for (size_t j = 0; j < count; j++) {
      line(image, contours[i][j], contours[i][(j + 1) % count],
           Scalar(0, 0, 255), 3);
    }
  }

  imshow("contour", image);
  waitKey();
}

int main() {
  Mat image = imread("../image/polygon.png");
  Mat gray, binary;
  cvtColor(image, gray, COLOR_BGR2GRAY);
  threshold(gray, binary, 100, 255, THRESH_BINARY);
  
  int scale = 2;
  Size zoom_size(image.cols * scale, image.rows * scale);
  Mat zoom(zoom_size, CV_8UC1), zoom_binary(zoom_size, CV_8UC1);
  resize(image, zoom, zoom_size);
  resize(binary, zoom_binary, zoom_size);

  draw_contour(image, binary);
  draw_contour(zoom, zoom_binary);
  detect_corner(image, binary);
  detect_corner(zoom, zoom_binary);
}
