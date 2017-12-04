#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

int main(int argc, char** argv) {
  VideoCapture capture = VideoCapture(argv[1]);
  if (!capture.isOpened())
    return -1;
  
  Mat image;
  capture >> image;
  double fps = capture.get(CV_CAP_PROP_FPS);
  Size size(
      static_cast<int> (capture.get(CV_CAP_PROP_FRAME_WIDTH)),
      static_cast<int> (capture.get(CV_CAP_PROP_FRAME_HEIGHT)));

  VideoWriter writer = VideoWriter(argv[2], CV_FOURCC('M', 'j', 'P', 'G'),
                                   fps, size);

  Mat logpolar_frame(size, IPL_DEPTH_8U, Scalar::all(255));

  while (!image.empty()) {
    writer << image;
    capture >> image;
  }

  capture.release();
  image.release();
}
