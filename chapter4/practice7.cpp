#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

Point2f point;
bool add_remove_pt = false;

static void onMouse(int event, int x, int y, int flags, void * param){
  if (event == EVENT_LBUTTONDOWN) {
    point = Point2f(x, y);
    add_remove_pt = true;
  }
}

int main(int argc, char** argv) {
  VideoCapture cap;
  TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
  Size sub_pix_win_size(10, 10), win_size(31, 31);

  const int MAX_COUNT = 500;
  bool need_to_init = false;
  bool night_mode = false;

  cv::CommandLineParser parser(argc, argv, "{@input|0|}");
  string input = parser.get<string>("@input");

  if (input.size() == 1 && isdigit(input[0]))
    cap.open(input[0] - '0');
  else
    cap.open(input);

  if (!cap.isOpened())
  {
    cout << "Could not initialize capturing...\n";
    return 0;
  }

  namedWindow("LK Demo");
  setMouseCallback("LK Demo", onMouse);

  Mat gray, prev_gray, frame;
  Mat image(1000, 1000, CV_8UC3, Scalar(0, 0, 0));
  vector<Point2f> points[2];
  Rect roi;
  int x_move = 0;
  int y_move = 0;
  
  for (;;) {
    cap >> frame;
    if (frame.empty()) break;
    
    image = Scalar::all(0);
    if (points[1].empty()) {
      roi = Rect(500 - frame.cols / 2, 500 - frame.rows / 2,
                 frame.cols, frame.rows);
    }
    else {
      roi = Rect(500 - frame.cols / 2 - x_move, 500 - frame.rows / 2 - y_move,
                 frame.cols, frame.rows);
    }
    
    frame.copyTo(image(roi));
    
    cvtColor(image, gray, COLOR_BGR2GRAY);
    if (night_mode)
      image = Scalar::all(0);

    if (need_to_init) {
      goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10);
      cornerSubPix(gray, points[1], sub_pix_win_size, Size(-1, -1), termcrit);
      add_remove_pt = false;
    }
    else if (!points[0].empty()) {
      vector<uchar> status;
      vector<float> err;
      int x_diff = 0;
      int y_diff = 0;
      if (prev_gray.empty())
        gray.copyTo(prev_gray);
      calcOpticalFlowPyrLK(prev_gray, gray, points[0], points[1], status, err,
                           win_size, 3, termcrit, 0, 0.001);
      size_t i, k;
      for (i = k = 0; i < points[1].size(); i++) {
        if (add_remove_pt) {
          if (norm(point - points[1][i]) <= 5) {
            add_remove_pt = false;
            continue;
          }
        }
        if (!status[i]) continue;

        x_diff += static_cast<int>(points[1][i].x - points[0][i].x);
        y_diff += static_cast<int>(points[1][i].y - points[0][i].y);
        points[1][k++] = points[1][i];
        circle(image, points[1][i], 3, Scalar(0, 255,0), -1, 8);
      }
      x_move += x_diff / static_cast<int>(k - 4);
      y_move += y_diff / static_cast<int>(k - 4);

      if (add_remove_pt && points[1].size() < static_cast<size_t>(MAX_COUNT)) {
        vector<Point2f> tmp;
        tmp.push_back(point);
        cornerSubPix(gray, tmp, win_size, Size(-1, -1), termcrit);
        points[1].push_back(tmp[0]);
        add_remove_pt =false;
      }
    }
    
    need_to_init = false;
    imshow("LK Demo", image);

    char c = static_cast<char>(waitKey(10));
    if (c == 27) break;
    switch (c)
    {
      case 'r':
        need_to_init = true;
        break;
      case 'c':
        points[0].clear();
        points[1].clear();
        break;
      case 'n':
        night_mode = !night_mode;
        break;
    }

    std::swap(points[1], points[0]);
    cv::swap(prev_gray, gray);
    
  }

  return 0;
}
