//g++ simpleImageRead2.cpp `pkg-config --libs opencv` -o simpleImageRead2

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(void)
{
	Mat disp = imread("sample.jpeg");
	namedWindow("Display Window for the Sample Image", WINDOW_NORMAL);
	imshow("Display Window for the Sample Image", disp);
	waitKey(0);
	destroyWindow("Display Window for the Sample Image");
}
	
