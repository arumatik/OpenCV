#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int main(){
  Mat m = (Mat_<double>(3,3) << 1, 2, 3, 7, 4, 5, 5, 6, 7);
  Mat c = m.inv();
  double det = determinant(m);
  cout << m << endl;
  cout << c << endl;
  cout << det << endl;
}
