#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


int main() {
  Mat image1 = imread("../image/magcup1.jpg");
  Mat image2 = imread("../image/magcup2.jpg");
  Mat outside = imread("../image/home1.jpg");
  Mat gray1, gray2, diff, dst, opening, image;

  while (image1.rows > 1000 || image1.cols > 1000) {
    pyrDown(image1, image1);
    pyrDown(image2, image2);
    pyrDown(outside, outside);
  }

  cvtColor(image1, gray1, COLOR_BGR2GRAY);
  cvtColor(image2, gray2, COLOR_BGR2GRAY);
  diff = abs(gray1 - gray2);
  threshold(diff, dst, 100, 255, THRESH_BINARY);
  morphologyEx(dst, opening, MORPH_OPEN, Mat());

  image = Mat(diff.rows, diff.cols * 3, CV_8UC1);
  Rect roi1(0, 0, diff.cols, diff.rows);
  Rect roi2(diff.cols, 0, diff.cols, diff.rows);
  Rect roi3(diff.cols * 2, 0, diff.cols, diff.rows);
  diff.copyTo(image(roi1));
  dst.copyTo(image(roi2));
  opening.copyTo(image(roi3));

  namedWindow("magcup");

  imshow("magcup", image);
  waitKey(0);

  int max_maskpix_num = 0;
  int max_mask_x, max_mask_y;
  for (int j = 0; j < opening.rows; ++j) {
    unsigned char * val = opening.ptr(j);
    
    for (int i = 0; i < opening.cols; ++i) {
      if (val[i] == 255) {
        int masked_pix_num = 0;
        floodFill(opening, Point(i, j), Scalar(100));
        opening.forEach<uchar>(
            [&masked_pix_num](uchar &x, const int *position) {
              if (x == 100) masked_pix_num += 1;
            });
        if (masked_pix_num - max_maskpix_num > max_maskpix_num) {
          if (max_maskpix_num > 0)
            floodFill(opening, Point(max_mask_x, max_mask_y), Scalar(0));
          max_mask_x = i;
          max_mask_y = j;
          max_maskpix_num = masked_pix_num;
        }
        else 
          floodFill(opening, Point(i,j), Scalar(0));
      }
    }
  }

  imshow("magcup", opening);
  waitKey(0);

  image2.copyTo(outside, opening);

  imshow("magcup", outside);
  waitKey(0);
  
  return 0;
}
