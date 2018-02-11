#include <iostream>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int main(){
  Mat A = Mat(5, 5, CV_32F, Scalar(1));
  imwrite("my_matrix.png", A);

  FileStorage fs("test.xml", FileStorage::WRITE);

  int frame_count = 0;
  fs << "frame_count" << frame_count;
  fs << "Matrix" << A;

  return 0;
}


