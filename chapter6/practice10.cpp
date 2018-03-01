#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

int main() {
  Mat kernel = getGaussianKernel(5, -1);
  Mat imker = Mat::zeros(kernel.size(), CV_32F);
  Mat kernel_planes[] = {Mat_<float>(kernel), imker};
  Mat compker;
  merge(kernel_planes, 2, compker);
  dft(compker, compker);

  Mat image = imread("../image/fruits.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  Mat padded, complex, dst;

  int m = getOptimalDFTSize(image.rows);
  int n = getOptimalDFTSize(image.cols);
  copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));
  
  Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
  merge(planes, 2, complex);
  dft(complex, complex);

  split(compker, kernel_planes);
  split(complex, planes);
  filter2D(planes[0], planes[0], -1, kernel_planes[0]);
  filter2D(planes[1], planes[1], -1, kernel_planes[1]);
  merge(planes, 2, complex);

  dft(complex, dst, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);

  namedWindow("fftgauss");
  imshow("fftgauss", padded);
  waitKey(0);
  imshow("fftgauss", dst / 255);
  waitKey(0);
  
  return 0;
}
