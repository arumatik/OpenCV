#include <vector>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


Mat source;
Rect box;
bool drawing_rect = false;
Mat b_histimg, g_histimg, r_histimg;

void draw_histogram(Mat roi) {
  Mat b_hist, g_hist, r_hist;
  int histsize = 8;
  float range[] = {0, 256};
  const float* histrange = {range};
  vector<Mat> bgr;
  vector<Mat> histogram(3, Mat());
  split(roi, bgr);
  
  calcHist(&bgr[0], 1, 0, Mat(), histogram[0], 1, &histsize, &histrange);
  calcHist(&bgr[1], 1, 0, Mat(), histogram[1], 1, &histsize, &histrange);
  calcHist(&bgr[2], 1, 0, Mat(), histogram[2], 1, &histsize, &histrange);

  int scale = 30;
  int pixel = source.rows * source.cols / 500;
  b_histimg = Mat(pixel, histsize * scale, CV_8UC3, Scalar(255, 255, 255));
  g_histimg = Mat(pixel, histsize * scale, CV_8UC3, Scalar(255, 255, 255));
  r_histimg = Mat(pixel, histsize * scale, CV_8UC3, Scalar(255, 255, 255));

  namedWindow("blue histogram");
  namedWindow("green histogram");
  namedWindow("red histogram");
  for (int ind = 0; ind < histsize; ++ind) {
    int b_height = histogram[0].at<float>(ind) / 100;
    int g_height = histogram[1].at<float>(ind) / 100;
    int r_height = histogram[2].at<float>(ind) / 100;
    if (b_height != 0) {
      Rect roi = Rect(ind * scale, pixel - b_height, scale, b_height);
      rectangle(b_histimg, roi, Scalar(255, 0, 0), CV_FILLED);
    }
    if (g_height != 0) {
      Rect roi = Rect(ind * scale, pixel - g_height, scale, g_height);
      rectangle(g_histimg, roi, Scalar(0, 255, 0), CV_FILLED);
    }
    if (r_height != 0) {
      Rect roi = Rect(ind * scale, pixel - r_height, scale, r_height);
      rectangle(r_histimg, roi, Scalar(0, 0, 255), CV_FILLED);
    }
  }
}

void mouse_callback(int event, int x, int y, int flags, void* userdata) {
  Mat image = *static_cast<Mat*>(userdata);
  Mat histogram;
  
  if (event == EVENT_LBUTTONDOWN) {
    source.copyTo(image);
    drawing_rect = true;
    box = Rect(x, y, 0, 0);
  }
  
  if (event == EVENT_MOUSEMOVE) {
    if (drawing_rect == true) {
      source.copyTo(image);
      box.width = x - box.x;
      box.height = y - box.y;
      rectangle(image, box.tl(), box.br(),  Scalar(0, 0, 0));
    }
  }

  if (event == EVENT_LBUTTONUP) {
    source.copyTo(image);
    drawing_rect = false;
    box.width = x- box.x;
    box.height = y - box.y;
    if (box.width < 0) {
        box.x += box.width;
        box.width *= -1;
    }
    if (box.height < 0) {
      box.y += box.height;
      box.height *= -1;
    }

    if (box.width > 0 & box.height > 0) {
      Mat roi = image(box);
      draw_histogram(roi);
      roi.forEach<Vec3b>([](Vec3b &p, const int * position) {
          p[0] = p[0] > 205 ? 255 : p[0] + 50;
          p[1] = p[1] > 205 ? 255 : p[1] + 50;
          p[2] = p[2] > 205 ? 255 : p[2] + 50;
        });
    }
  }
}


int main() {
  source = imread("../image/fruits.jpg");
  Mat image = source.clone();
  namedWindow("highlight");
  setMouseCallback("highlight", mouse_callback, &image);
  
  while (1) {
    imshow("highlight", image);
    if (!b_histimg.empty()) imshow("blue histogram", b_histimg);
    if (!g_histimg.empty()) imshow("green histogram", g_histimg);
    if (!r_histimg.empty()) imshow("red histogram", r_histimg);
    if (waitKey(15) == 27) break;
  }
  image.release();
  source.release();
  destroyAllWindows();
}
