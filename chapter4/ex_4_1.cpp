#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

bool drawing_box = false;
Rect box;

void my_mouse_callback(int event, int x, int y, int flags, void* param);

void draw_box(Mat& img, Rect box){
  rectangle(img, box.tl(), box.br(), Scalar(255, 0, 0));
}


int main() {
  box = Rect(-1, -1, 0, 0);
  Mat image = Mat(200, 200, CV_8UC3, Scalar(0, 0, 0));
  Mat tmp = image.clone();
  
  namedWindow("call back function");
  
  setMouseCallback("call back function", my_mouse_callback,
                   static_cast<void*>(&image));

  while (1) {
    image.copyTo(tmp);
    if (drawing_box) draw_box(tmp, box);
    imshow("call back function", tmp);

    if (waitKey(15) == 27) break;
  }
  image.release();
  tmp.release();
  destroyAllWindows();
}

void my_mouse_callback(int event, int x, int y, int flags, void* param) {
  Mat image = *static_cast<Mat*>(param);

  switch (event) {
    case EVENT_MOUSEMOVE: {
      if (drawing_box) {
        box.width = x - box.x;
        box.height = y - box.y;
      }
    }
    break;
    case EVENT_LBUTTONDOWN: {
      drawing_box = true;
      box = Rect(x, y, 0, 0);
    }
    break;
    case EVENT_LBUTTONUP: {
      drawing_box = false;
      if (box.width < 0) {
        box.x += box.width;
        box.width *= -1;
      }
      if (box.height < 0) {
        box.y += box.height;
        box.height *= -1;
      }
      draw_box(image, box);
    }
    break;
  }
}
