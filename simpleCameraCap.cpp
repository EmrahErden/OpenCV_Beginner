//g++ simpleCameraCap.cpp `pkg-config --libs opencv` -o simpleCameraCap

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main (void)
{
	VideoCapture cap;
	Mat frame;
	namedWindow("Window", WINDOW_AUTOSIZE);
	cap.open(0);
	if(!cap.isOpened())
	{
		cerr << "Couldn't open the capture!\n";
	}
	while(1)
	{
		cap >> frame;
		imshow("Video", frame);
		waitKey(10);
	}
}
