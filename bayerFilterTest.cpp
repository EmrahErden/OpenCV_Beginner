//g++ bayerFilterTest.cpp `pkg-config --libs opencv` -o bayerFilterTest

#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>

using namespace cv;

int main(void)
{
	namedWindow("original", WINDOW_AUTOSIZE);
	namedWindow("blue", WINDOW_AUTOSIZE);
	namedWindow("green", WINDOW_AUTOSIZE);
	namedWindow("red", WINDOW_AUTOSIZE);

	Mat input = imread("bayerPattern.jpg");
	vector<Mat> inputChannels(3);
	cv::split(input, inputChannels);
	Mat& blueChannel = inputChannels[0];
	Mat& greenChannel = inputChannels[1];
	Mat& redChannel = inputChannels[2];

	imshow("original", input);
	imshow("red", redChannel);
	imshow("green", greenChannel);
	imshow("blue", blueChannel);
	waitKey(0);
}
