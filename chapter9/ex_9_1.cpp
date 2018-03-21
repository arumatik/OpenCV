#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


int main() {
  VideoCapture cap(0);
  Mat raw_image;
  int max_buffer;
  vector<vector<Vec3b> > bgr_buffer;

  for (;;) {
    cap >> raw_image;
    if (raw_image.empty()) break;
    LineIterator itr(raw_image, Point(raw_image.cols / 2 - 100, 100),
                     Point(raw_image.cols / 2 + 100, 100));
    max_buffer = itr.count;
    vector<Vec3b> buffer(max_buffer);
    
    for (int i = 0; i < max_buffer; i++, ++itr) {
      buffer[i] = Vec3b(*itr);
    }
    bgr_buffer.push_back(buffer);
    
    imshow("cam", raw_image);
    if (waitKey(15) == 27) break;
  }

}
