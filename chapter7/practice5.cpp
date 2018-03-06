#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


int gridnum = 16;
int channels[] = {0, 1, 2};
float range[] = {0, 255};
int histsize[] = {gridnum, gridnum, gridnum};
const float * ranges[] = {range, range, range};


void calc_histogram(const Mat & image, Mat & hist){
  calcHist(&image, 1, channels, Mat(), hist, 3, histsize, ranges);
}


Mat make_model_histogram(const string filename) {
  Mat model = imread(filename);
  Mat model_hist;
  Rect roi(model.cols / 2 - 50, model.rows / 2 - 50, 100, 100);
  //imshow("model", model(roi));
  //waitKey();
  calc_histogram(model(roi), model_hist);
  normalize(model_hist, model_hist, 0, 255, NORM_MINMAX);
  return model_hist;
}


bool file_exists(const string & fname) {
  ifstream fs(fname);
  return fs.is_open();
}


int make_webcam_capture_image(const string & filestr) {
  Mat frame;
  VideoCapture webcam(0);
  int idx = 0;
  string filename;
  
  while (1) {
    filename = filestr + to_string(idx) + ".jpg";
    if (!file_exists(filename)) break;
    idx++;
  }

  namedWindow("Live");
  while(1) {
    webcam.read(frame);
    imshow("Live", frame);
    int key = waitKey(5);
    if (key == 27) break;
    else if (key == 13) {
      filename = filestr + to_string(idx) + ".jpg";
      imwrite(filename, frame);
      idx++;
      cout << "save file : " << filename << endl;
      waitKey(30);
    }
  }
  destroyAllWindows();
  return idx;
}


void detect_roi(const Mat & image, Mat & dst, const Mat & model_hist){
  calcBackProject(&image, 1, channels, model_hist, dst, ranges);
  //imshow("back project", dst);
  //waitKey();

  Mat mask(dst.rows + 2, dst.cols + 2, CV_8UC1, Scalar::all(0));
  for (int j = 0; j < dst.rows; j++) {
    unsigned char * row_val = dst.ptr(j);
    unsigned char * row_mask = mask.ptr(j);
    for (int i = 0; i < dst.cols; i++) {
      if (row_val[i] >= 32 && row_mask[i] == 0){
        Rect rect;
        floodFill(dst, mask, Point(i, j), row_val[i] / 16 * 16 , &rect, 8, 8);
      }
    }
  }

  imshow("floodfill", dst);
  waitKey();
  return;
}


void make_differential_hist(const Mat & image, Mat & diff_hist){
  Mat planes[2] = {Mat(image.size(), CV_8UC1)}, dx, dy;
  int diffchannel[] = {0, 1};
  
  Sobel(image, dx, CV_16S, 1, 0);
  Sobel(image, dy, CV_16S, 0, 1);
  
  double mini[1];
  minMaxLoc(dx, mini);
  normalize(dx + abs(static_cast<int>(*mini)), planes[0], 255, 0, NORM_MINMAX);
  minMaxLoc(dy, mini);
  normalize(dy + abs(static_cast<int>(*mini)), planes[1], 255, 0, NORM_MINMAX);
  
  diff_hist = Mat(gridnum,  gridnum, CV_32FC1, Scalar::all(0));
  //calcHist(planes, 2, diffchannel, Mat(), diff_hist, 2, histsize, ranges);
  int count = 0;
  for (int j = 0; j < image.rows; j++) {
    unsigned char * dxs = planes[0].ptr<unsigned char>(j);
    unsigned char * dys = planes[1].ptr<unsigned char>(j);
    for (int i = 0; i < image.cols; i++) {
      if (dx.at<signed char>(j, i) != 0 && dy.at<signed char>(j, i) != 0) {
        int xidx = cvRound(dxs[i] / gridnum);
        int yidx = cvRound(dys[i] / gridnum);
        diff_hist.at<float>(yidx, xidx) += 1;
        count++;
      }
    }
  }
  
  int scale = 30;
  Mat hist_img(diff_hist.rows * scale, diff_hist.cols * scale,
               CV_8UC1, Scalar::all(0));
  for (int y = 0; y < diff_hist.rows; y++) {
    for (int x = 0; x < diff_hist.cols; x++) {
      Rect roi(Point(x * scale, y * scale),
               Point((x + 1) * scale - 1, (y + 1) * scale - 1));
      rectangle(hist_img, roi,
                Scalar::all(diff_hist.at<float>(y, x)), -1);
    }
  }
  imshow("differential hist", hist_img);
  waitKey(0);
  destroyAllWindows();
  return;
}

int main() {
  string filestr = "../image/HandIndoorwebcam";
  int idx = make_webcam_capture_image(filestr);

  Mat model_hist(3, histsize, CV_32FC1, Scalar::all(0));
  for (int i = 0; i < idx; i++) {
    string filename = filestr + to_string(i) + ".jpg";
    model_hist += make_model_histogram(filename);
  }
  normalize(model_hist, model_hist, 0, 255, NORM_MINMAX);
  
  Mat up_image = imread("../image/HandIndoorwebcam0.jpg");
  Mat detected_up, up_diff_hist;
  detect_roi(up_image, detected_up, model_hist);
  make_differential_hist(detected_up, up_diff_hist);
  
  Mat pall_image = imread("../image/HandIndoorwebcam7.jpg");
  Mat detected_pall, pall_diff_hist;
  detect_roi(pall_image, detected_pall, model_hist);
  make_differential_hist(detected_pall, pall_diff_hist);
  
  Mat target = imread("../image/HandIndoorwebcam9.jpg");
  Mat target_roi, target_diff_hist;
  detect_roi(target, target_roi, model_hist);
  make_differential_hist(target_roi, target_diff_hist);
  
  double corre_up = compareHist(up_diff_hist, target_diff_hist, HISTCMP_CORREL);
  double corre_pall = compareHist(
      pall_diff_hist, target_diff_hist, HISTCMP_CORREL);
  cout << corre_up << endl;
  cout << corre_pall << endl;
  return 0;
}
