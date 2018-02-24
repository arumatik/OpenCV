#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

int main() {
  Mat image = imread("../image/microstructure.jpg");
  Mat pyramid, geo;

  resize(image, geo, Size((image.cols + 1) / 2, (image.rows + 1) / 2));
  resize(geo, geo, Size((geo.cols + 1) / 2, (geo.rows  + 1) / 2));
  resize(geo, geo, Size((geo.cols + 1) / 2, (geo.rows  + 1) / 2));

  pyrDown(image, pyramid);
  pyrDown(pyramid, pyramid);
  pyrDown(pyramid, pyramid);

  Mat output(geo.rows, geo.cols * 2, CV_8UC3);
  geo.copyTo(output(Rect(0, 0, geo.cols, geo.rows)));
  pyramid.copyTo(output(Rect(geo.cols, 0, geo.cols, geo.rows)));

  namedWindow("resize");
  imshow("resize", output);

  waitKey(0);

  return 0;
}
