#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <iostream>
using namespace std;
using namespace cv;

int g_slider_position = 0;

void onTrackbarSlide(int pos, void*) {
}

Mat doPyrDown(Mat& in) {
  // assert(in.cols%2 == 0 && in.rows%2 == 0);
  Mat out(Size(in.cols/2, in.rows/2),
          in.depth(), Scalar::all(255));
  pyrDown(in, out);
  return out;
}

int main(int argc, char** argv) {
  // argv[1] : output filename
  VideoCapture capture = VideoCapture(0);
  Mat image;
  double fps = capture.get(CV_CAP_PROP_FPS);
  Size size(static_cast<int> (capture.get(CV_CAP_PROP_FRAME_WIDTH) / 2),
            static_cast<int> (capture.get(CV_CAP_PROP_FRAME_HEIGHT)) / 2);
  VideoWriter writer(argv[1], CV_FOURCC('M', 'J', 'P', 'G'), fps, size);
  Size out_size;
  namedWindow("practice", CV_WINDOW_NORMAL);

  createTrackbar("Level", "practice", &g_slider_position, 2, onTrackbarSlide);

  while (1) {
    capture >> image;
    for(int i=0; i <= g_slider_position; i++)
      image = doPyrDown(image);
    imshow("practice", image);
    char c = waitKey(10);
    if (c == 27)
      break; // ESC
  }

  capture.release();
  image.release();
  destroyAllWindows();
}
