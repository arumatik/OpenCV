#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main() {
  Mat image = imread("../image/bicycle.jpg");
  Mat gray, dst;
  vector<Vec4i> lines;
  vector<Vec3f> circles;

  cvtColor(image, gray, COLOR_BGR2GRAY);
  Canny(image, dst, 50, 200, 3);
  
  HoughLinesP(dst, lines, 1, CV_PI / 180, 80, 30, 10);
  HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, dst.rows / 4, 200, 200,
               10, dst.rows / 4);

  for (size_t i = 0; i < lines.size(); ++i) {
    line(image, Point(lines[i][0], lines[i][1]),
         Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
  }

  for (size_t i = 0; i < circles.size(); ++i) {
    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    int radius = cvRound(circles[i][2]);
    circle(image, center, radius, Scalar(0, 255, 0), 3, 8, 0);
  }

  namedWindow("Hough transformation");
  imshow("Hough transformation", dst);
  waitKey(0);
  
  imshow("Hough transformation", image);
  waitKey(0);
  
  return 0;
}
