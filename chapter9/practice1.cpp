#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


int main () {
  VideoCapture cap(0);
  Mat frame, fframe, mask, dst;
  Mat background, accumdiff, difference, previous, high, low;
  float count = 0.;
  float alpha = 0.1;
  float scale = 5.;

  for (;;) {
    cap >> frame;
    frame.convertTo(fframe, CV_32F, 1.0 / 255);

    if (background.empty()) {
      background = fframe.clone();
      accumdiff = Mat(frame.size(), CV_32FC3, Scalar::all(0));
      difference = Mat(frame.size(), CV_32FC3, Scalar::all(0));
    }
    background = (1 - alpha) * background + alpha * fframe;
    if (!previous.empty()) {
      accumdiff += abs(fframe - previous); 
      difference = accumdiff / count;
    }
    previous = fframe.clone();
    count += 1.;

    high = background + scale * difference;
    low = background - scale * difference;

    inRange(fframe, low, high, mask);
    mask = 255 - mask;

    cvtColor(mask, mask, COLOR_GRAY2BGR);
    bitwise_and(frame, mask, dst);

    imshow("frame", frame);
    imshow("bg remove", dst);
    if (waitKey(10) != -1) break;
  }
}
