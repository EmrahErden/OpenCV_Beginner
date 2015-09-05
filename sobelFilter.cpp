//g++ sobelFilter.cpp `pkg-config --libs opencv` -o sobelFilter

/*
The compass edge detectors, of which the two best known examples are Sobel and Prewitt
Both are first derivative edge detectors. Using first order derivatives.
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int main(void)
{
	Mat img = imread("sample.jpeg"), gray_image;
	cvtColor(img, gray_image, CV_8UC1);
	namedWindow("horizontal_derivative", WINDOW_AUTOSIZE);
	namedWindow("vertical_derivative", WINDOW_AUTOSIZE);
	Mat horizontal_derivative, vertical_derivative;
	Sobel( gray_image, horizontal_derivative, CV_32F ,1,0 );
	Sobel( gray_image, vertical_derivative, CV_32F,0,1 );
	imshow("horizontal_derivative", horizontal_derivative);
	imshow("vertical_derivative", vertical_derivative);

	/*	
	In OpenCV, given the two partial derivatives we may compute the absolute gradient and
	the l2norm (gradient) and orientation
	*/
	Mat abs_gradient, l2norm_gradient, orientation;
	abs_gradient = abs(horizontal_derivative) + abs(vertical_derivative);
	cartToPolar(horizontal_derivative,vertical_derivative,
	l2norm_gradient, orientation);

	waitKey(0);
}


