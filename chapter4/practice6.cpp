#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

string label;
Mat source;

void write_label(int x, int y, Mat & img) {
  while (1) {
    source.copyTo(img);
    int val = waitKey(15);
    if (val == 13) {
      putText(img, label, Point(x, y), FONT_HERSHEY_SIMPLEX, 1,
              Scalar(0, 0, 0));
      imshow("label", img);
      img.copyTo(source);
      label.resize(0);
      break;
    }
    if (val == 8) label.erase(label.end() - 1);
    else if (val != -1) label += val; 
    putText(img, label, Point(x, y), FONT_HERSHEY_SIMPLEX, 1,
            Scalar(0, 0, 0));
    imshow("label", img);
  }
}


void label_callback(int event, int x, int y, int flags, void * param){
  Mat img = *static_cast<Mat*>(param);
  if (event == EVENT_LBUTTONUP) {
    write_label(x, y, img);
  }
}

int main() {
  source = Mat(500, 500, CV_8UC3, Scalar(255, 255, 255));
  Mat image = source.clone();

  namedWindow("label");
  setMouseCallback("label", label_callback, &image);

  while (1) {
    imshow("label", image);
    if (waitKey(15) == 27) break;
  }

  image.release();
  destroyAllWindows();
}
