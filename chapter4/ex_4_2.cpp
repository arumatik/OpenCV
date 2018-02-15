#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int g_switch_value = 0;

void switch_off_function() {
  Mat img = imread("../image/fruits.jpg");
  imshow("switching", img);
}

void switch_on_function() {
  Mat img = imread("../image/stuff.jpg");
  imshow("switching", img);
}

void switch_callback(int position, void* userdata=0) {
  if (position == 0) {
    switch_off_function();
  } else {
    switch_on_function();
  }
}


int main() {
  namedWindow("switching");
  createTrackbar("switch", "switching", &g_switch_value, 1, switch_callback);
  switch_callback(g_switch_value);

  while (1) {
    if (waitKey(15) == 27) break;
  }

  destroyAllWindows();
}
