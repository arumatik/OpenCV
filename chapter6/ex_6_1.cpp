#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;


int main() {
  Mat image = imread("../image/fruits.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  Mat kernel = getGaussianKernel(5, -1);
  Mat dst;
  Mat padded;

  namedWindow("dft");

  int m = getOptimalDFTSize(image.rows);
  int n = getOptimalDFTSize(image.cols);

  copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
  Mat complex;
  merge(planes, 2, complex);

  imshow("dft", planes[0] / 255.);
  waitKey(0);

  dft(complex, complex);

  split(complex, planes);
  magnitude(planes[0], planes[1], planes[0]);
  Mat magni = planes[0];

  imshow("dft", planes[0] / 255.);
  waitKey(0);

  imshow("dft", planes[1] / 255.);
  waitKey(0);

  imshow("dft", magni / 255.);
  waitKey(0);

  dft(complex, dst, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);

  imshow("dft", dst / 255.);
  waitKey(0);

  return 0;
}
