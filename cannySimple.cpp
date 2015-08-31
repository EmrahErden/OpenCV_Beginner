//g++ cannySimple.cpp `pkg-config --libs opencv` -o cannySimple

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

int main(void)
{
	Mat imgOrg = imread("sample.jpeg"), imgGray, imgCanny;
	namedWindow("Window1", WINDOW_NORMAL);
	namedWindow("Window2", WINDOW_NORMAL);
	cvtColor(imgOrg, imgGray, COLOR_BGR2GRAY);
	Canny(imgGray, imgCanny, 10, 100, 3, true);
	imshow("Window1", imgOrg);
	imshow("Window2", imgCanny);
	waitKey(0);
}

