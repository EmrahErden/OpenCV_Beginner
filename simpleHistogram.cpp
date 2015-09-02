//g++ simpleHistogram.cpp `pkg-config --libs opencv` -o simpleHistogram

#include "opencv2/highgui/highgui.hpp"

using namespace cv;

//histogram display for gray-scale images
Mat computeGSHist(Mat & input)
{
	const int * channel_numbers = { 0 };
	float channel_range[] = { 0.0, 255.0 };
	const float * channel_ranges = channel_range;
	int number_bins = 64;
	Mat histogram;
	calcHist( &input, 1, channel_numbers, Mat(), histogram,
	1, &number_bins, &channel_ranges);

	return histogram;
}

int main(void)
{
	Mat img = imread("sample.jpeg");
	Mat imgGS, hist;
	img.convertTo(imgGS, CV_8UC3);
	hist = computeGSHist(imgGS);
}
