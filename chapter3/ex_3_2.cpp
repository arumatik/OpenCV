#include <iostream>
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
    Rect_<int> interest_rect1(x, y, height, width);
    Rect_<int> interest_rect2(x+100, y+100, height, width);
    Mat roi1 = img(interest_rect1);
    Mat roi2 = img(interest_rect2);
    add(roi1, Scalar(add_), roi1);
    add(roi2, Scalar(add_+100), roi2);

    namedWindow("Interest Rectangle", CV_WINDOW_AUTOSIZE);
    imshow("Interest Rectangle", img);

    waitKey(0);
    img.release();
    destroyAllWindows();
  }
}
