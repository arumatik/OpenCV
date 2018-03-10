#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


void draw_convex(Mat & image, vector<vector<Point> > & contour,
                 vector<Vec4i> defect, Mat & source) {
  vector<int> hull;
  findContours(image, contour, RETR_EXTERNAL, CHAIN_APPROX_NONE);
  convexHull(contour[0], hull, false, false);
  convexityDefects(contour[0], hull, defect);
  
  for (size_t i = 0; i < hull.size(); i++) {
    line(source, Point(contour[0][hull[i]]),
         Point(contour[0][hull[(i + 1) % hull.size()]]), Scalar(0, 0, 255), 3);
  }
  imshow("hull", source);
  waitKey();
}

int main() {
  Mat image = imread("../image/polygon.png");
  Mat gray, binary;
  double angle = 60.;
  double scale = 2.;
  Mat zoom, rotate, zoom_rotate;
  vector<vector<Point> > b_contour, z_contour, r_contour, zr_contour;
  vector<Vec4i> b_defect, z_defect, r_defect, zr_defect;

  cvtColor(image, gray, COLOR_BGR2GRAY);
  threshold(gray, binary, 100, 255, THRESH_BINARY);

  resize(binary, zoom, Size(binary.cols * scale, binary.rows * scale));
  Mat affin = getRotationMatrix2D(Point(binary.cols / 2, binary.rows / 2),
                                  angle, 1.);
  warpAffine(binary, rotate, affin, binary.size());
  resize(rotate, zoom_rotate, Size(rotate.cols * scale, rotate.rows * scale));

  draw_convex(binary, b_contour, b_defect, image);
}
