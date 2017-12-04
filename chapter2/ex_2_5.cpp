#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

Mat doPyrDown(Mat& in) {
  // assert(in.cols%2 == 0 && in.rows%2 == 0);

  Mat out(Size(in.cols/2, in.rows/2), in.depth(), Scalar::all(255));
  pyrDown(in, out);
  return out;
}

Mat doCanny(Mat& in, double lowTresh,
            double highTresh, double aperture) {
  if (in.channels()!= 1) {
    cout << "channels : " << in.channels() << endl;
    //return 0;
  }

  Mat out(cvSize(in.cols/2, in.rows/2), in.depth(), Scalar::all(255));
  Canny(in, out, lowTresh, highTresh, aperture);
  return out;
}

int main(int argc, char** argv) {
  // IplImage* img = cvLoadImage(argv[1]);
  int level = 2;
  Mat img = imread(argv[1]);
  Mat out = doPyrDown(img);
  Mat out2 = doPyrDown(out);
  //Mat out = doCanny(img, 100, 0, 3);
  namedWindow("Example5", CV_WINDOW_AUTOSIZE);
  imshow("Example5", img);
  namedWindow("Example5-after", CV_WINDOW_AUTOSIZE);
  imshow("Example5-after", out);
  namedWindow("Example5-after2", CV_WINDOW_AUTOSIZE);
  imshow("Example5-after2", out2);

  waitKey(0);
  img.release();
  out.release();
  destroyAllWindows();
}
