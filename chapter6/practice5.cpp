#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main() {
  Mat image = imread("../image/face_front.jpg");
  Mat gray, laplace, thresh;
  
  namedWindow("face");
  
  cvtColor(image, gray, COLOR_BGR2GRAY);
  imshow("face", gray);
  waitKey(0);
  
  gray = 255 - gray;
  imshow("face", gray);
  waitKey(0);

  Laplacian(gray, laplace, -1, 5);
  imshow("face", laplace);
  waitKey(0);

  return 0;
}
