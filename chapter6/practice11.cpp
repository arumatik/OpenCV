#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int argc, char ** argv) {
  Mat image = imread(argc > 1 ? argv[1] : "../image/bicycle.jpg",
                     CV_LOAD_IMAGE_GRAYSCALE);
  Mat sum_img;
  integral(image, sum_img);

  namedWindow("integral");
  imshow("integral", sum_img);
  waitKey(0);
  
  return 0;
}
