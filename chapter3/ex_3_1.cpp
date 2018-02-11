#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
  Mat img = imread(argv[1]);
  
  if (argc == 7 && !img.empty()) {
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    int width = atoi(argv[4]);
    int height = atoi(argv[5]);
    int add_ = atoi(argv[6]);
    Rect_<int> roi_coordi(x, y, height, width);
    Mat roi = img(roi_coordi);
    add(roi, Scalar(add_), roi);
    
    namedWindow("ROI Add", CV_WINDOW_AUTOSIZE);
    imshow("ROI Add", img);
    
    waitKey(0);
    img.release();
    destroyAllWindows();
  }
  
  return 0;
}
