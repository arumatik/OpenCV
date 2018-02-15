#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat image = imread("../image/fruits.jpg");

void display_mouse_loc(int event, int x, int y, int flags, void* userdata) {
  Mat tmp = *static_cast<Mat*>(userdata);
  if (event == EVENT_LBUTTONDOWN) {
    image.copyTo(tmp);
    string loc = to_string(x) + ", " + to_string(y);
    putText(tmp, loc, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
  }
}

int main () {
  Mat tmp = image.clone();

  namedWindow("practice");
  setMouseCallback("practice", display_mouse_loc, static_cast<void*>(&tmp));

  while (1) {
    imshow("practice", tmp);
    if (waitKey(15) == 27) break;
  }

  image.release();
  tmp.release();
  destroyAllWindows();
}
