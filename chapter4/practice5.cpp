#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


int old_x, old_y;
bool painting = 0;
bool erasing = 0;
int slider_pos = 0;
int old_slider_pos = 0;
vector<Point> poly_pts;
Mat source;


void draw_line(int event, int x, int y, Mat & img) {
  if (event == EVENT_LBUTTONDOWN) {
    painting = 1;
    old_x = x;
    old_y = y;
    old_slider_pos = slider_pos;
  }

  if (event == EVENT_MOUSEMOVE) {
    if (painting) {
      if (old_x != x & old_y != y){
        line(img, Point(old_x, old_y), Point(x, y), Scalar(0, 0, 0), 3);
        old_x = x;
        old_y = y;
      }
    }
  }

  if (event == EVENT_LBUTTONUP) {
    painting = 0;
  }
  img.copyTo(source);
}


void draw_circle(int event, int x, int y, Mat & img) {
  if (event == EVENT_LBUTTONDOWN) {
    painting = 1;
    old_x = x;
    old_y = y;
    old_slider_pos = slider_pos;
  }
  
  if (event == EVENT_MOUSEMOVE) {
    if (painting) {
      source.copyTo(img);
      if (old_x != x & old_y != y) {
        double radius = pow(pow(old_x - x, 2) + pow(old_y - y, 2), 1. / 2);
        circle(img, Point(old_x, old_y), radius, Scalar(0, 0, 0));
      }
    }
  }

  if (event == EVENT_LBUTTONUP) {
    source.copyTo(img);
    if (old_x != x & old_y != y) {
      double radius = pow(pow(old_x - x, 2) + pow(old_y - y, 2), 1. / 2);
      circle(img, Point(old_x, old_y), radius, Scalar(0, 0, 0));
    }
    painting = 0;
    img.copyTo(source);
  }
}


void draw_ellipse(int event, int x, int y, Mat & img) {
  if (event == EVENT_LBUTTONDOWN) {
    painting = 1;
    old_x = x;
    old_y = y;
    old_slider_pos = slider_pos;
  }
  
  if (event == EVENT_MOUSEMOVE) {
    if (painting) {
      source.copyTo(img);
      if (old_x != x & old_y != y) {
        Point_<float> center = Point_<float>((old_x + x) / 2., (old_y + y)/ 2.);
        Size_<float> size = Size_<float>(abs(old_x - x), abs(old_y - y));
        RotatedRect box = RotatedRect(center, size, 0);
        ellipse(img, box, Scalar(0, 0, 0));
      }
    }
  }

  if (event == EVENT_LBUTTONUP) {
    source.copyTo(img);
    if (old_x != x & old_y != y) {
      Point_<float> center = Point_<float>((old_x + x) / 2., (old_y + y)/ 2.);
      Size_<float> size = Size_<float>(abs(old_x - x), abs(old_y - y));
      RotatedRect box = RotatedRect(center, size, 0);
      ellipse(img, box, Scalar(0, 0, 0));
    }
    painting = 0;
    img.copyTo(source);
  }
}


void draw_polygon(int event, int x, int y, Mat & img) {
  if (event == EVENT_LBUTTONUP) {
    source.copyTo(img);
    old_slider_pos = slider_pos;
    poly_pts.push_back(Point(x, y));
    if (poly_pts.size() > 2) {
      Point *pts = reinterpret_cast<Point*>(Mat(poly_pts).data);
      int npts = poly_pts.size();
      polylines(img, &pts, &npts, 1, true, Scalar(0, 0, 0));
    }
  }
}


void paint_mouse_callback(int event, int x, int y, int flags, void* param) {
  Mat img = *static_cast<Mat*>(param);
  if (old_slider_pos != slider_pos & old_slider_pos == 3) img.copyTo(source);

  switch (event) {
    case EVENT_RBUTTONDOWN:
      {
        erasing = 1;
        old_x = x;
        old_y = y;
      }
      break;
    case EVENT_RBUTTONUP:
      erasing = 0;
      break;
    case EVENT_MOUSEMOVE:
      {
        if (erasing) {
          if (old_x != x & old_y != y){
            line(img, Point(old_x, old_y), Point(x, y),
                 Scalar(255, 255, 255), 3);
            old_x = x;
            old_y = y;
          }
        }
      }
    default:
      {
        switch (slider_pos) {
          case 0:
            draw_line(event, x, y, img);
            break;
          case 1:
            draw_circle(event, x, y, img);
            break;
          case 2:
            draw_ellipse(event, x, y, img);
            break;
          case 3:
            {
              if (old_slider_pos != slider_pos) poly_pts.resize(0);
              draw_polygon(event, x, y, img);
            }
            break;
        }
      }
  }
}


void draw_pattern_slider(int position, void*){
  slider_pos = position;
}


int main() {
  int height = 500;
  int width = 500;
  source = Mat(height, width, CV_8UC3, Scalar(255, 255, 255));
  Mat image = source.clone();

  namedWindow("paint");
  setMouseCallback("paint", paint_mouse_callback, &image);
  createTrackbar("pattern", "paint", &slider_pos, 3, draw_pattern_slider);

  while (1) {
    imshow("paint", image);
    if (waitKey(15) == 27) break;
  }

  image.release();
  destroyAllWindows();
}
