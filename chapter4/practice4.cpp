#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

bool pause = 0;
VideoCapture video;
int frame_count;
int d_slide;

void slider_trackbar(int position, void*) {
  video.set(CV_CAP_PROP_POS_FRAMES, d_slide * position);
}


void pause_trackbar(int position, void*) {
  pause = position;
}

int main() {
  video = VideoCapture("../image/test.avi");
  frame_count = video.get(CV_CAP_PROP_FRAME_COUNT);
  Mat frame;
  int s_default = 0;
  d_slide = frame_count / 10;
  video >> frame;
  
  namedWindow("video slider");
  createTrackbar("position", "video slider", &s_default, 10, slider_trackbar);
  createTrackbar("start/stop", "video slider", &s_default, 1, pause_trackbar);
  
  while (!frame.empty()) {
    int pos = video.get(CV_CAP_PROP_POS_FRAMES);
    if (pos % d_slide == 0) {
      setTrackbarPos("position", "video slider", pos / d_slide);
    }
    imshow("video slider", frame);
    while (pause) {
      waitKey(15);
    }
    waitKey(15);
    video >> frame;
  }

  video.release();
  frame.release();
  destroyAllWindows();
}
