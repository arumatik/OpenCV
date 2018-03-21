#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;


int main(int argc, char ** argv) {
  Mat image = imread(argc > 1 ? argv[1] : "../image/fruits_inpaint.jpg");
  Mat mean_shift, dst;

  pyrMeanShiftFiltering(image, mean_shift, 16, 32, 2);


  Mat mask(mean_shift.size(), CV_8UC1, Scalar::all(0));
  mask.forEach<uchar>([&mean_shift] (uchar & p, const int * position) -> void {
      Vec3b pix = mean_shift.at<Vec3b>(position);
      if (pix[0] < 5 || pix[1] < 5 || pix[2] < 5) p = 255;
    });

  inpaint(mean_shift, mask, dst, 5, INPAINT_NS);

  imshow("source", image);
  imshow("mean shift filter", mean_shift);
  imshow("mask", mask);
  imshow("dst", dst);
  waitKey();
}
