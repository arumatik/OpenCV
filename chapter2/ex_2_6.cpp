#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

VideoCapture capture;

int main(int argc, char** argv) {
  if (argc==1) {
    capture = VideoCapture(0);
  }
  else {
    capture = VideoCapture(argv[1]);
  }
  // assert(capture != NULL);

  Mat image;
  namedWindow("Example6", CV_WINDOW_AUTOSIZE);
  
  while (1) {
  capture >> image;
  if (image.empty()) break;
  imshow("Example6", image);
  char c = waitKey(33);
  if (c == 27) break;
  }

  image.release();
  destroyAllWindows();
}
