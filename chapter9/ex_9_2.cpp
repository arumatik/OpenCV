#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


Mat average, previous, difference;
Mat high[3], low[3];
float image_count = 0.0;
float alpha = 1.0;

void set_high_threshold(float);
void set_low_threshold(float);


void accumulate_background(Mat & image) {
  Mat fimage, diff;
  image.convertTo(fimage, CV_32FC3);
  if (average.empty()) {
    fimage.copyTo(average);
    fimage.copyTo(previous);
    difference = Mat::zeros(image.size(), CV_32FC3);
  } else {
    average += alpha * fimage;
    absdiff(fimage, previous, diff);
    difference += alpha * diff;
  }
  image_count += alpha;
  fimage.copyTo(previous);
}


void create_model_from_stats() {
  average /= image_count;
  difference /= image_count;
  image_count = 1.0;
  difference += 1.;
  set_high_threshold(3.0);
  set_low_threshold(3.0);
}


void set_high_threshold(float scale) {
  Mat high_image;
  high_image = difference * scale + average;
  split(high_image, high);
}


void set_low_threshold(float scale) {
  Mat low_image;
  low_image = average - difference * scale;
  split(low_image, low);
}


Mat background_diff(Mat & image) {
  Mat fimage, single_mask;
  Mat mask = Mat::zeros(image.size(), CV_8UC1);
  Mat gray[3];
  image.convertTo(fimage, CV_32FC3);
  split(fimage, gray);

  for (int i = 0; i < 3; i++){
    inRange(gray[i], low[i], high[i], single_mask);
    bitwise_or(mask, single_mask, mask);
  }
  return mask;
}


int main() {
  VideoCapture cap(0);
  Mat image, mask, dst;

  for (;;) {
    cap >> image;
    accumulate_background(image);
    imshow("image", image);
    if (waitKey(10) != -1) break;
  }
  create_model_from_stats();
  destroyAllWindows();

  alpha = 0.2;
  int j = 0;
  for (;;j++) {
    cap >> image;
    mask = background_diff(image);
    threshold(mask, mask, 1, 255, THRESH_BINARY_INV);
    cvtColor(mask, mask, COLOR_GRAY2BGR);
    bitwise_and(image, mask, dst);
    imshow("foreground", dst);
    if (waitKey(10) != -1) break;

    if (j % 30 == 0) {
      accumulate_background(image);
      create_model_from_stats();
    }
  }
}
