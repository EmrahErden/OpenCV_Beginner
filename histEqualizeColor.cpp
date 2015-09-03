//g++ histEqualizeColor.cpp `pkg-config --libs opencv` -o histEqualizeColor

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

void equalizeHistColor(Mat & input, Mat & output)
{
	vector<Mat> inputChannels(3);
	vector<Mat> outputChannels(3);
	split(input, inputChannels);
	for(int i=0; i<3; i++)
	{
		equalizeHist(inputChannels[i], outputChannels[i]);
	}
	merge(outputChannels, output);
}

int main(void)
{
	Mat input = imread("sample.jpeg");
	Mat output = input.clone();
	namedWindow("input", WINDOW_AUTOSIZE);
	namedWindow("output", WINDOW_AUTOSIZE);
	equalizeHistColor(input, output);
	imshow("input", input);
	imshow("output", output);
	waitKey(0);
}
