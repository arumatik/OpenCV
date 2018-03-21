#include "ex_9_4.hpp"

void find_connected_components(
    Mat & mask, int & num, vector<Rect> & bbs, vector<Point> & centers,
    int poly1_hull0, float perim_scale) {
  vector<vector<Point> > contours;
  Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
  morphologyEx(mask, mask, MORPH_OPEN, kernel, Point(-1, -1), CVCLOSE_ITR);
  morphologyEx(mask, mask, MORPH_CLOSE, kernel, Point(-1, -1), CVCLOSE_ITR);

  findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

  for (auto itr = contours.begin(); itr != contours.end();) {
    double len = arcLength(*itr, true);
    double q = (mask.cols + mask.rows) / perim_scale;

    if (len < q) {
      itr = contours.erase(itr);
    } else {
      vector<Point> applox;
      if (poly1_hull0)
        approxPolyDP(*itr, applox, CVCONTOUR_APPROX_LEVEL, 0);
      else 
        convexHull(*itr, applox, true);
      *itr = applox;
      itr++;
    }
  }
  
  const Scalar WHITE(255, 255, 255);
  const Scalar BLACK(255, 255, 255);

  mask = Mat::zeros(mask.size(), mask.type());
  Mat mask_temp;

  if (num != 0) {
    int num_filled = 0;
    Moments moms;
    mask_temp = mask.clone();
    for (size_t i = 0; i < contours.size(); i++) {
      if (static_cast<int>(i) < num) {
        drawContours(mask_temp, contours, i, WHITE, FILLED);
        moms = moments(mask_temp, true);
        centers.push_back(Point(static_cast<int>(moms.m10 / moms.m00),
                                static_cast<int>(moms.m01 / moms.m00)));
        bbs.push_back(boundingRect(contours[i]));
        mask_temp = Mat::zeros(mask.size(), mask.type());
        num_filled++;
      }
      drawContours(mask, contours, i, WHITE, FILLED);
    }
    num = num_filled;
  }
  else 
    drawContours(mask, contours, -1, WHITE, FILLED);
}
