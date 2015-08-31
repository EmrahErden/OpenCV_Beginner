//g++ pyrDownSimple.cpp `pkg-config --libs opencv` -o pyrDownSimple
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main(void)
{
	namedWindow("Window1",WINDOW_NORMAL);
	namedWindow("Window2",WINDOW_NORMAL);
	Mat img1 = imread("sample.jpeg"), img2;
	pyrDown(img1, img2);
	imshow("Window1",img1);
	imshow("Window2",img2);
	waitKey(0);
}
