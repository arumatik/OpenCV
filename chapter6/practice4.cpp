#include <iostream>
#include <vector>
#include <numeric>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main() {
  Mat image(300, 300, CV_8UC1, Scalar(0));
  line(image, Point(0, image.rows), Point(image.cols, 0),
       Scalar(255, 255, 255), 10);

  namedWindow("white line");
  imshow("white line", image);
  waitKey(0);

  Mat dstx, dsty;
  Mat mag(image.rows, image.cols, CV_64FC1, Scalar(0));
  Mat theta(image.rows, image.cols, CV_64FC1, Scalar(0));
  double max_val = 0;
  vector<double> max_theta;
  int ksize = 3;
  Sobel(image, dstx, -1, 1, 0, ksize);
  Sobel(image, dsty, -1, 0, 1, ksize);


  for (int j = 0; j < image.rows; ++j) {
    unsigned char * dxrow = dstx.ptr<unsigned char>(j);
    unsigned char * dyrow = dsty.ptr<unsigned char>(j);
    for (int i = 0; i < image.cols; ++i) {
      double magnitude = pow(pow(dxrow[i], 2) + pow(dyrow[i], 2), 1. / 2);
      if (magnitude > max_val)
        max_val = magnitude;
      mag.at<double>(j, i) = magnitude;
      theta.at<double>(j, i) = atan(1. * dyrow[i] / dxrow[i]);
    }
  }

  for (int j = 0; j < image.rows; ++j) {
    double * magrow = mag.ptr<double>(j);
    double * therow = theta.ptr<double>(j);
    for (int i = 0; i < image.cols; ++i) {
      if (magrow[i] == max_val){
        //cout << "dx : " << mag.at<double>(j, i) << ", dy : " <<
        //    mag.at<double>(j, i) << ", theta : " <<
        //    therow[i] << endl;
        max_theta.push_back(therow[i]);
      }
    }
  }

  cout << accumulate(max_theta.begin(), max_theta.end(), 0.) / max_theta.size()
       << endl;
  
  return 0;
}
