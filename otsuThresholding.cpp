//g++ otsuThresholding.cpp `pkg-config --libs opencv` -o otsuThresholding

/*
While for some industrial applications thresholds are set manually, this can cause problems
over time as lighting changes throughout the day. Even within industrial lighting enclosures,
the lighting sources gradually become weaker over time. Hence, mechanisms are required to
automatically determine thresholds (Sezgin and Sankur, 2004).
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int main(void)
{
	Mat original = imread("otsuBinaryThreshold.png");
	Mat gray;
	Mat binary;
	int thresholdVal = 0; //Note that in this case the passed thresholdVal value is ignored.
	namedWindow("binary image", WINDOW_AUTOSIZE);
	cvtColor(original, gray, CV_BGR2GRAY);
	threshold( gray, binary, thresholdVal,
	255, THRESH_BINARY | THRESH_OTSU );
	imshow("binary image", binary);
	waitKey(0);
}
