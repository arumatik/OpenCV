#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

int main() {
  Mat img1 = imread("../image/fruits.jpg");
  Mat img2 = Mat(200, 300, img1.type(), &img1.at<double>(5, 10), img1.step);
  Mat img3 = Mat(300, 200, img1.type(), &img1.at<double>(50, 60), img1.step);
  
  bitwise_not(img2, img2);
  namedWindow("practice", CV_WINDOW_AUTOSIZE);
  imshow("practice", img2);
  waitKey(0);
  img2.release();

  bitwise_not(img3, img3);
  namedWindow("practice", CV_WINDOW_AUTOSIZE);
  imshow("practice", img3);
  waitKey(0);
  img3.release();


  namedWindow("practice", CV_WINDOW_AUTOSIZE);
  imshow("practice", img1);

  waitKey(0);
  img1.release();

  destroyAllWindows();
}
