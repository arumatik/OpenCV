#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "ex_9_3.cpp"
using namespace cv;


int main() {
  Mat msf;
  
  for (int j = 0;;j++) {
    cap >> frame;
    msf = frame;
    //pyrMeanShiftFiltering(frame, msf, 8, 8, 3);
    msf.forEach<Vec3b>([&](Vec3b & p, const int * position) -> void {
        int idx = position[0] + position[1] * frame.rows;
        update_codebook(p, codebooks[idx], cb_bounds, CHANNELS);
        codebooks[idx].t++;
      });
    if (j % 255 == 0) {
      for (size_t i = 0; i < codebooks.size(); i++)
        clear_stale_entries(codebooks[i]);
    }
    imshow("webcam", frame);
    if (waitKey(10) != -1) break;
  }
  
  Mat mask(frame.size(), CV_8UC1, Scalar::all(0));
  Mat raw_fore, connected_fore, raw_mask, connected_mask;
  for (int j = 0;;j++) {
    cap >> frame;
    //pyrMeanShiftFiltering(frame, msf, 8, 8, 1);
    msf.forEach<Vec3b>([&](Vec3b & p, const int * position) -> void {
        int idx = position[0] + position[1] * frame.rows;
        mask.at<uchar>(position[0], position[1]) =
            background_diff(p, codebooks[idx], CHANNELS, max_mod, min_mod);
        codebooks[idx].t++;
        if (j % 64 == 0)
          update_codebook(p, codebooks[idx], cb_bounds, CHANNELS);
        if (j % 255 == 0)
          clear_stale_entries(codebooks[idx]);
      });
    mask = 255 - mask;
    
    cvtColor(mask, raw_mask, COLOR_GRAY2BGR);
    bitwise_and(msf, raw_mask, raw_fore);

    /*
    vector<Rect> bbs;
    vector<Point> centers;
    int num = 0;
    find_connected_components(mask, num, bbs, centers, 0, 10);
    cvtColor(mask, connected_mask, COLOR_GRAY2BGR);
    bitwise_and(frame, connected_mask, connected_fore);
    */

    imshow("webcam", msf);
    imshow("raw", raw_fore);
    //imshow("connected", connected_fore);
    if (waitKey(10) != -1) break;
  }

    
}
