#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;


int main() {
  vector<Mat> image;
  int height = 300;
  int width = 300;
  for (int i = 0; i < 4; ++i) {
    image.push_back(Mat(height, width, CV_64FC1, Scalar(0, 0, 0)));
  }
  
  rectangle(image[0], Point(50, 50), Point(250, 250), Scalar(255, 255, 255));
  rectangle(image[1], Point(50, 10), Point(250, 210), Scalar(255, 255, 255));

  RotatedRect rect1(Point(150, 150), Size(200, 200), 45);
  RotatedRect rect2(Point(150, 150), Size(100, 100), 45);
  Point2f pts1[4];
  Point2f pts2[4];
  rect1.points(pts1);
  rect2.points(pts2);
  
  for (int i = 0; i < 4; ++i) {
    line(image[2], pts1[i], pts1[(i + 1) % 4], Scalar(255, 255, 255));
    line(image[3], pts2[i], pts2[(i + 1) % 4], Scalar(255, 255, 255));
  }

  Mat logp, padded, complex, dst;
  int m = getOptimalDFTSize(height);
  int n = getOptimalDFTSize(width);
  Point2f center(width / 2, height / 2);
  
  for (int i = 0; i < 4; ++i) {
    copyMakeBorder(image[i], padded, (m - image[i].rows) / 2,
                   (m - image[i].rows) / 2, (n - image[i].cols) / 2,
                   (n - image[i].cols) / 2, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    merge(planes, 2, complex);
    
    dft(complex, complex, 0);
    split(complex, planes);
    magnitude(planes[0], planes[1], planes[0]);
    
    imshow("logpolar and fourier", planes[0]);
    waitKey(0);
    
    dft(planes[0], dst, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
    imshow("logpolar and fourier", dst / 100);
    waitKey(0);
    
    logPolar(dst, logp, center, 20, INTER_LINEAR);
    imshow("logpolar and fourier", logp / 100);
    waitKey(0);
  }
  
  namedWindow("logpolar and fourier");
  
  return 0;
}
