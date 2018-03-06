#include <iostream>
#include <cstdlib>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char ** argv) {
  Mat sample;
  Mat indoor = imread("../image/HandIndoorColor2.jpg");
  Mat outdoor1 = imread("../image/HandOutdoorColor.jpg");
  Mat outdoor2 = imread("../image/HandOutdoorSunColor.jpg");
  sample = indoor(Rect(Point(350, 400), Point(450, 700)));

  int channels[] = {0, 1, 2};
  int rgbsize = argc > 1 ? atoi(argv[1]) : 16;
  int histsize[] = {rgbsize, rgbsize, rgbsize};
  float range[] = {0, 256};
  const float * ranges[] = {range, range, range};
  Mat sample_hist, target_hist;
  calcHist(&sample, 1, channels, Mat(), sample_hist, 3, histsize, ranges);
  calcHist(&indoor, 1, channels, Mat(), target_hist, 3, histsize, ranges);
  normalize(sample_hist, sample_hist, 0, 255, NORM_MINMAX, -1, Mat());
  normalize(target_hist, target_hist, 0, 255, NORM_MINMAX, -1, Mat());

  double correla = compareHist(sample_hist, target_hist, HISTCMP_CORREL);
  cout << "correlation : " << correla << endl;
  
  double chisqr = compareHist(sample_hist, target_hist, HISTCMP_CHISQR);
  cout << "CHI square : " << chisqr << endl;
  
  double inter = compareHist(sample_hist, target_hist, HISTCMP_INTERSECT);
  cout << "Intersect : " << inter << endl;
  
  double bhatta = compareHist(sample_hist, target_hist, HISTCMP_BHATTACHARYYA);
  cout << "BHATTACHARYYA distance : " << bhatta << endl;

  Mat backproj;
  calcBackProject(&indoor, 1, channels, sample_hist, backproj, ranges, 1, true);

  imshow("sample", sample);
  imshow("target", indoor);
  imshow("backproj", backproj);
  waitKey(0);
  
  return 0;
}
