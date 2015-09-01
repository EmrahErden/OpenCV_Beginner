//g++ addGaussianNoise.cpp `pkg-config --libs opencv` -o addGaussianNoise

#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main(void)
{
	double average = 0.0;
	double standard_deviation = 30.0;
	namedWindow("image", WINDOW_AUTOSIZE);
	Mat image8bit = imread("sample.jpeg");
	Mat image16bit, output16bit, output8bit;
	Mat noise(image8bit.size(), CV_16SC3);
	randn(noise, Scalar::all(average), Scalar::all(standard_deviation));
	image8bit.convertTo(image16bit, CV_16SC3);
	addWeighted(noise, 1.0, image16bit, 1.0, 0.0, output16bit);
	output16bit.convertTo(output8bit, CV_8UC3);
	imshow("image", output8bit);
	waitKey(0);
}
