#include <iostream>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int rgbsize = 16;
int channels[] = {0, 1, 2};
float range[] = {0, 255};
int histsize[] = {rgbsize, rgbsize, rgbsize};
const float * ranges[] = {range, range, range};


void calc_histogram(const Mat & image, Mat & hist){
  calcHist(&image, 1, channels, Mat(), hist, 3, histsize, ranges);
}


Mat make_model_histogram(const string filename) {
  Mat model = imread(filename);
  Mat model_hist;
  Rect roi(model.cols / 2 - 50, model.rows / 2 - 50,
           model.cols / 2 + 50, model.rows / 2 + 50);
  calc_histogram(model(roi), model_hist);
  normalize(model_hist, model_hist, 0, 255, NORM_MINMAX);
  return model_hist;
}

int main(int argc, char ** argv) {
  string fname[] = {"../image/HandIndoorColor.jpg",
                    "../image/HandIndoorColor2.jpg",
                    "../image/HandIndoorColor3.jpg"};
  Mat model_hist(3, histsize, CV_32FC1, Scalar::all(0));
  for (string filename : fname)
    model_hist += make_model_histogram(filename);
  normalize(model_hist, model_hist, 0, 255, NORM_MINMAX);

  Mat inimage = imread(argc > 1 ? argv[1] : "../image/HandIndoorColor.jpg");
  Mat bp, bin_img;

  calcBackProject(&inimage, 1, channels, model_hist, bp, ranges);
  imshow("bp", bp);
  waitKey();

  Mat eroded = bp.clone();
  //erode(bp, eroded, Mat());
  //imshow("bp", eroded);
  //waitKey();
  
  //int min_entry = 30;
  int count = 0, count_old = 0, min_entry = 30;
  Mat mask(eroded.rows + 2, eroded.cols + 2, CV_8UC1, Scalar::all(0));
  for (int j = 0; j < eroded.rows; j++) {
    unsigned char * vals = eroded.ptr(j);
    unsigned char * masked = mask.ptr(j);
    for (int i = 0; i < eroded.cols; i++) {
      if (vals[i] >= 80 && masked[i] == 0) {
        Rect rect;
        floodFill(eroded, mask, Point(i, j), Scalar(255), &rect,
                  Scalar(20), Scalar(185));
      }
    }
  }
  cout << "end loop" << endl;
  imshow("floodfill", eroded);
  waitKey();

  return 0;
}

