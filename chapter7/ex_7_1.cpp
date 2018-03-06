#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int main(int argc, char ** argv) {
  string filename = argc > 1 ? argv[1] : "../image/fruits.jpg";
  Mat image = imread(filename);
  Mat hsv, hist;
  
  cvtColor(image, hsv, COLOR_BGR2HSV);

  int hbin = 30, sbin = 32;
  int histsize[] = {hbin, sbin};
  float hranges[] = {0, 180};
  float sranges[] = {0, 256};
  const float * ranges[] = {hranges, sranges};
  int channels[] = {0, 1};

  calcHist(&hsv, 1, channels, Mat(), hist, 2, histsize, ranges, true, false);

  double max_val = 0;
  minMaxLoc(hist, 0, &max_val, 0, 0);
  
  int scale = 10;
  Mat hist_img = Mat::zeros(hbin * scale, sbin * scale, CV_8UC3);

  for (int h = 0; h < hbin; h++) {
    for (int s = 0; s < sbin; s++) {
      float bin_val = hist.at<float>(h, s);
      int intensity = cvRound(bin_val * 255 / max_val);
      rectangle(hist_img, Point(h * scale, s * scale),
                Point((h + 1) * scale - 1, (s + 1) * scale - 1),
                Scalar::all(intensity), CV_FILLED);
    }
  }

  namedWindow("source");
  imshow("source", image);

  namedWindow("H-S Histogram");
  imshow("H-S Histogram", hist_img);
  waitKey();

  return 0;
}
