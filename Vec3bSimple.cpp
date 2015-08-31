//g++ Vec3bSimple.cpp `pkg-config --libs opencv` -o Vec3bSimple

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void getPixelValue(const Mat& img, int xpos, int ypos)
{
	Vec3b intensity = img.at<Vec3b>(ypos, xpos);
	uchar blue = intensity.val[0];
	uchar green = intensity.val[1];
	uchar red = intensity.val[2];
	cout << "number of channels in the image: " << img.channels() << "\n";
	cout << "B-G-R values in the specified location: ";
	cout << int(blue) << ", " << int(green) << ", " << int(red) << ", " << "\n";

}
int main(void)
{
	Mat img = imread("sample.jpeg");
	namedWindow("Window", WINDOW_AUTOSIZE);
	getPixelValue(img, 20, 20);
	imshow("Window", img);
	waitKey(0);
}
