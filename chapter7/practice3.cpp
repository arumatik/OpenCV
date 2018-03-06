#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


void calc_vhistogram(const Mat & image, Mat & bg_hist, Mat & roi_hist){
  Mat hsv;
  cvtColor(image, hsv, COLOR_BGR2HSV);
  Mat planes[3];
  split(hsv, planes);
  Mat background = planes[2](Rect(0, 0, 100, 100));
  Mat roi = planes[2](Rect(hsv.cols / 2 - 50, hsv.rows / 2 - 50,
                           hsv.cols / 2 + 50, hsv.rows / 2 + 50));
  int channels[] = {0};
  int histsize = 16;
  float range[] = {0, 255};
  const float * ranges[] = {range};
  calcHist(&background, 1, channels, Mat(), bg_hist, 1, &histsize, ranges);
  calcHist(&roi, 1, channels, Mat(), roi_hist, 1, &histsize, ranges);
}


double compare_sample_and_target(const Mat & s_roi, const Mat & t_roi,
                                 const Mat & s_bg, const Mat & t_bg) {
  double roi_correla = compareHist(s_roi, t_roi, HISTCMP_CORREL);
  double bg_correla = compareHist(s_bg, t_bg, HISTCMP_CORREL);
  return roi_correla + bg_correla;
}


int main(int argc, char ** argv) {
  Mat indoor = imread("../image/HandIndoorColor.jpg");
  Mat outdoor = imread("../image/HandOutdoorColor.jpg");
  Mat bg_in, bg_out, roi_in, roi_out;
  calc_vhistogram(indoor, bg_in, roi_in);
  calc_vhistogram(outdoor, bg_out, roi_out);
  
  Mat image = imread(argc > 1 ? argv[1] : "../image/HandIndoorColor2.jpg");
  Mat bg_hist, roi_hist;
  calc_vhistogram(image, bg_hist, roi_hist);

  double in_corre = compare_sample_and_target(roi_in, roi_hist, bg_in, bg_hist);
  double out_corre = compare_sample_and_target(
      roi_out, roi_hist, bg_out, bg_hist);

  cout << "in correlation : " << in_corre << endl;
  cout << "out correlation : " << out_corre << endl;

  if (in_corre > out_corre)
    cout << "Inside" << endl;
  else
    cout << "Outside" << endl;
  
  return 0;
}
