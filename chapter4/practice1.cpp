#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
  VideoCapture video = VideoCapture("../image/test.avi");

  Mat image, gray, canny;
  Mat all;
  
  video >> image;
  cvtColor(image, gray, CV_RGB2GRAY);
  Canny(gray, canny, 100, 200);
  all = Mat(image.rows + 25, image.cols * 3, CV_8UC3);
  putText(all, "source video", Point(0, image.rows + 20),
          FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255));
  putText(all, "gray scale", Point(image.cols, image.rows + 20),
          FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255));
  putText(all, "canny edge", Point(image.cols * 2, image.rows + 20),
          FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255));
  Mat roi0 = all(Rect(0, 0, image.cols, image.rows));
  Mat roi1 = all(Rect(image.cols, 0, image.cols, image.rows));
  Mat roi2 = all(Rect(image.cols * 2, 0, image.cols, image.rows));
  
  namedWindow("video");
  namedWindow("gray scale");
  namedWindow("Canny edge");
  namedWindow("All");

  while (! image.empty()) {
    imshow("video", image);
    imshow("gray scale", gray);
    imshow("Canny edge", canny);
    image.copyTo(roi0);
    cvtColor(gray, roi1, CV_GRAY2RGB);
    cvtColor(canny, roi2, CV_GRAY2RGB);
    imshow("All", all);
    if (waitKey(10) == 27) break;
    
    video >> image;
    cvtColor(image, gray, CV_RGB2GRAY);
    Canny(gray, canny, 100, 200);
  }
  while (1) {
    if (waitKey(0) == 27) break;
  }
  
  image.release();
  destroyAllWindows();
}
