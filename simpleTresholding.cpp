//g++ simpleTresholding.cpp `pkg-config --libs opencv` -o simpleTresholding

/*
Often grey-level 255 is used instead of binary 1 (so that the resulting image can be rep-
resented using a 8-bit format and displayed/processed in the same manner as the original
grey-scale image).
The most efficient means of implementing this operation (which can often be done in
hardware) is to use a lookup table.
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int main(void)
{
	Mat original = imread("sample.jpeg");
	Mat gray;
	vector<Mat> binary(5);
	namedWindow("binary image", WINDOW_AUTOSIZE);
	cvtColor(original, gray, CV_BGR2GRAY);
	for(int i=0; i <5; i++)
	{
		threshold(gray, binary[i], 255-i*50, 255, THRESH_BINARY);
		imshow("binary image", binary[i]);
		waitKey(0);
	}
}

//at each press to a random key on keyboard, threshold value changes so does the 
//output image displayed
