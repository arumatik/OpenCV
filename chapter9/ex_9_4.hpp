#ifndef EX_9_4
#define EX_9_4

#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


const int CVCONTOUR_APPROX_LEVEL = 2;
const int CVCLOSE_ITR = 1;


void find_connected_components(
    Mat & mask,
    int & num,
    vector<Rect> & bbs,
    vector<Point> & centers,
    int poly_hull0 = 1,
    float perim_scale = 4);

#endif
