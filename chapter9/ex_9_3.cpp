#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "ex_9_4.hpp"
using namespace std;
using namespace cv;

const int CHANNELS = 3;


struct code_element {
  uchar learn_high[CHANNELS];
  uchar learn_low[CHANNELS];
  uchar max[CHANNELS];
  uchar min[CHANNELS];
  int t_last_update;
  int stale;
};


struct code_book {
  vector<code_element> cb;
  int num_entries;
  int t;
};


VideoCapture cap(0);
Mat frame;
code_book c;
vector<code_book> codebooks(640 * 480);
vector<int> cb_bounds {10, 10, 10};
vector<int> max_mod {10, 10, 10};
vector<int> min_mod {10, 10, 10};


int update_codebook(Vec3b p, code_book & c,
                    vector<int> cb_bounds, int num_channels) {
  vector<unsigned int> high(3), low(3);
  for (int n = 0; n < num_channels; n++) {
    high[n] = p[n] + cb_bounds[n];
    if (high[n] > 255) high[n] = 255;
    low[n] = p[n] - cb_bounds[n];
    if (low[n] < 0) low[n] = 0;
  }

  int i, match_channel;
  for (i = 0; i < c.num_entries; i++) {
    match_channel = 0;
    for (int n = 0; n < num_channels; n++) {
      if ((c.cb[i].learn_low[n] <= p[n]) && p[n] <= c.cb[i].learn_high[n]) {
        match_channel++;
      }
    }
    if (match_channel == num_channels) {
      c.cb[i].t_last_update = c.t;
      for (int n = 0; n < num_channels; n++) {
        if (c.cb[i].max[n] < p[n]) c.cb[i].max[n] = p[n];
        else if (c.cb[i].min[n] > p[n]) c.cb[i].min[n] = p[n];
      }
      break;
    }
  }
  
  if (i == c.num_entries) {
    code_element new_cl;
    for (int n = 0; n < num_channels; n++) {
      new_cl.learn_high[n] = high[n];
      new_cl.learn_low[n] = low[n];
      new_cl.max[n] = p[n];
      new_cl.min[n] = p[n];
    }
    new_cl.t_last_update = c.t;
    new_cl.stale = 0;
    c.cb.push_back(new_cl);
    c.num_entries = c.cb.size();
  }
  
  for (int s = 0; s < c.num_entries; s++) {
    int neg_run = c.t - c.cb[s].t_last_update;
    if (c.cb[s].stale < neg_run) c.cb[s].stale = neg_run;
  }

  for (int n = 0; n < num_channels; n++) {
    if (c.cb[i].learn_high[n] < high[n]) c.cb[i].learn_high[n] += 1;
    if (c.cb[i].learn_low[n] < low[n]) c.cb[i].learn_low[n] -= 1;
  }

  return i;
}

int clear_stale_entries(code_book & c) {
  int stale_thresh = c.t >> 1;
  vector<int> keep(c.num_entries);

  for (auto itr = c.cb.begin(); itr != c.cb.end();)
    if ((*itr).stale > stale_thresh) {
      itr = c.cb.erase(itr); 
    }
    else {
      (*itr).t_last_update = 0;
      itr++; 
    }
  c.t = 0;

  int num_cleared = c.num_entries - c.cb.size();
  c.num_entries = c.cb.size();
  return num_cleared;
}


uchar background_diff(Vec3b p, code_book & c, int num_channels,
                      vector<int> max_mod, vector<int> min_mod) {
  int i, match_channel;
  for (i = 0; i < c.num_entries; i++) {
    match_channel = 0;
    for (int n = 0; n < num_channels; n++) {
      if ((c.cb[i].min[n] - min_mod[n] <= p[n]) &&
          (p[n] <= c.cb[i].max[n] + max_mod[n])) {
        match_channel++;
      } else break;
    }
    if (match_channel == num_channels) break;
  }
  if (i >= c.num_entries) return 0;
  return 255;
}

void background_model(){
  for (int j = 0;;j++) {
    cap >> frame;
    frame.forEach<Vec3b>([&](Vec3b & p, const int * position) -> void {
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
}


void test_foreground() {
  Mat mask(frame.size(), CV_8UC1, Scalar::all(0));
  Mat raw_fore, connected_fore, raw_mask, connected_mask;
  for (int j = 0;;j++) {
    cap >> frame;
    frame.forEach<Vec3b>([&](Vec3b & p, const int * position) -> void {
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
    bitwise_and(frame, raw_mask, raw_fore);

    /*
    vector<Rect> bbs;
    vector<Point> centers;
    int num = 0;
    find_connected_components(mask, num, bbs, centers, 0, 10);
    cvtColor(mask, connected_mask, COLOR_GRAY2BGR);
    bitwise_and(frame, connected_mask, connected_fore);
    */
    
    imshow("webcam", frame);
    imshow("raw", raw_fore);
    //imshow("connected", connected_fore);
    if (waitKey(10) != -1) break;
  }
}

/*
int main() {
  background_model();
  test_foreground();
}
*/
