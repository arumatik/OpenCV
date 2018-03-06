#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


void make_emd_signature(const Mat & hist, Mat & signa, int bgrsize) {
  for (int x = 0; x < bgrsize; x++) {
    for (int y = 0; y < bgrsize; y++) {
      for (int z = 0; z < bgrsize; z++){
        int idx[] = {x, y, z};
        float entry = hist.at<float>(idx);
        float * src = signa.ptr<float>(x * bgrsize * bgrsize + y * bgrsize + z);
        src[0] = entry;
        src[1] = x;
        src[2] = y;
        src[3] = z;
      }
    }
  }
}


int main() {
  Mat sample;
  Mat img1 = imread("../image/HandIndoorColor2.jpg");
  sample = img1(Rect(Point(350, 400), Point(450, 700)));
  img1 = imread("../image/HandIndoorColor.jpg");
  //img1 = imread("../image/HandOutdoorColor.jpg");

  int channels[] = {2, 1, 0};
  Mat samplehist, hist1;
  int bgrsize = 4;
  int histsize[] = {bgrsize, bgrsize, bgrsize};
  float range[] = {0, 256};
  const float * ranges[] = {range, range, range};

  calcHist(&sample, 1, channels, Mat(), samplehist, 3, histsize, ranges); 
  calcHist(&img1, 1, channels, Mat(), hist1, 3, histsize, ranges); 

  normalize(samplehist, samplehist, 1.0, 0.0, NORM_L1);
  normalize(hist1, hist1, 1.0, 0, NORM_L1);

  double minval = 255;
  double maxval = 0;
  samplehist.forEach<float>([&](float & p, const int * position) -> void {
      minval = minval > p ? p : minval;
      maxval = maxval > p ? maxval : p;
    });
  cout << "minimum : " << minval << ", maximum : " << maxval << endl;
  
  int emdrow = bgrsize * bgrsize * bgrsize;
  Mat samplesig(emdrow, 4, CV_32FC1);
  Mat sig1(emdrow, 4, CV_32FC1);

  make_emd_signature(samplehist, samplesig, bgrsize);
  make_emd_signature(hist1, sig1, bgrsize);

  float emd1 = EMD(samplesig, sig1, DIST_L1);

  cout << "emd : " << emd1 << endl;
  
  Mat emd_img(img1.rows, img1.cols, CV_32FC1);
  img1.forEach<Vec3b>([&](Vec3b & p, const int * position) -> void {
      Mat pix(1, 1, CV_8UC3, Scalar(p[0], p[1], p[2]));
      Mat pixhist;
      Mat pixsig(emdrow, 4, CV_32FC1);
      calcHist(&pix, 1, channels, Mat(), pixhist, 3, histsize, ranges);
      make_emd_signature(pixhist, pixsig, bgrsize);
      float pixemd = EMD(samplesig, pixsig, DIST_L1);
      emd_img.at<float>(position[0], position[1])
          = pixemd < emd1 ? 1. / pixemd : 0;
    });

  normalize(emd_img, emd_img, 0, 1, NORM_MINMAX);
  
  imshow("calc_emd", emd_img);
  waitKey(0);
      
  return 0;
}
