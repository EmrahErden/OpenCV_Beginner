//g++ simpleFilter.cpp `pkg-config --libs opencv` -o simpleFilter

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

void gaussianFilter(Mat & img)
{
	Mat out;
	namedWindow("Window", WINDOW_NORMAL);
	GaussianBlur( img, out, Size(5,5), 3, 3);
	GaussianBlur( out, out, Size(5,5), 3, 3);
	imshow("Window", out);
	waitKey(0);
}

int main(void)
{
	Mat img = imread("sample.jpeg");
	gaussianFilter(img);
}
