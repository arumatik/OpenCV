#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

int main() {
  Mat img = imread("../image/fruits.jpg");
  Mat mv[img.channels()];
  split(img, mv);
  
  Mat clone1, clone2;
  mv[1].copyTo(clone1);
  mv[1].copyTo(clone2);

  int max_val {0};
  int min_val {255};
  for (int i = 0; i < mv[1].rows; i++) {
    unsigned char* itr = mv[1].ptr<unsigned char>(i);
    for (int j = 0; j < mv[1].cols; j++) {
      max_val =  itr[j] > max_val ? itr[j] : max_val;
      min_val =  itr[j] < min_val ? itr[j] : min_val;
    }
  }
  cout << "max value : " << max_val << endl;
  cout << "min value : " << min_val << endl;

  int thresh = (max_val - min_val) / 2.0;
  cout << "thresh : " << thresh << endl;

  clone1.forEach<unsigned char>([thresh] (unsigned char &p, const int * position) ->
                                void {
                                  p = thresh;
                                });
  clone2.forEach<unsigned char>([] (unsigned char &p, const int * position) ->
                                void {
                                  p = 0;
                                });

  compare(mv[1], clone1, clone2, CMP_GE);
  subtract(mv[1], thresh/2, mv[1], clone2);
  
  namedWindow("practice", CV_WINDOW_AUTOSIZE);
  imshow("practice", mv[1]);
  waitKey(0);
  img.release();
  
  namedWindow("practice", CV_WINDOW_AUTOSIZE);
  imshow("practice", clone1);
  waitKey(0);
  img.release();
  
  namedWindow("practice", CV_WINDOW_AUTOSIZE);
  imshow("practice", clone2);
  waitKey(0);
  img.release();
  destroyAllWindows();
}
