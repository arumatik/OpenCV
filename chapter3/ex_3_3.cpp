#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
  Mat img1, img2;
  img1 = imread(argv[1]);
  img2 = imread(argv[2]);
  if (argc == 9 && !img1.empty() && !img2.empty()) {
    int x = atoi(argv[3]);
    int y = atoi(argv[4]);
    int rows = atoi(argv[5]);
    int cols = atoi(argv[6]);
    double alpha = atof(argv[7]);
    double beta = atof(argv[8]);
    double gamma = 0;
    Rect interest_rect1(x, y, rows, cols);
    Rect interest_rect2(0, 0, rows, cols);
    Mat roi1 = img1(interest_rect1);
    Mat roi2 = img2(interest_rect2);
    addWeighted(roi1, alpha, roi2, beta, gamma, roi1);
    
    Mat transpose = img1.t();
    
    Point start(100, 200), goal(300, 400);
    line(transpose, start, goal, Scalar(0, 0, 255), 10);
    
    namedWindow("alpha blend", CV_WINDOW_AUTOSIZE);
    imshow("alpha blend", transpose);

    waitKey(0);
    img1.release();
    destroyAllWindows();
  }
}
