#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int main(){
  double negative = -12.5;
  cout << "number : " << negative << endl;
  cout << "abs : " << abs(negative) << endl;
  cout << "round : " << round(negative) << endl;

  RNG rng;
  for (int i = 0; i < 10; i++) {
    double random = rng.uniform(-100., 100.);
    cout << "uniform random : " << random << endl;
    cout << "gaussian random :  " << rng.gaussian(80.) << endl;
  }

  Mat random_matrix = Mat(5, 5, CV_32SC2);
  rng.fill(random_matrix, rng.UNIFORM, 10, 20);
  cout << random_matrix << endl;

  double x, y;
  x = rng.gaussian(100.);
  y = rng.gaussian(100.);
  cout << x << " " << y << endl;
  Point2d pt1 = Point2d(x, y);
  cout << pt1 << endl;
  Point pt2 = (Point)pt1;
  cout << pt2 << endl;
  return 0;
}
