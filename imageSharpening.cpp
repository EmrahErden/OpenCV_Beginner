//g++ imageSharpening.cpp `pkg-config --libs opencv` -o imageSharpening

/*
t is possible to use the second derivative to sharpen images, by simply subtracting a fraction
(e.g. 3/10) of the second derivative from the original image. This has the effect of lengthening
the edges
dawson-howe
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;

int main(void)
{
	Mat image = imread("licencePlate.jpg"), gray_image_32F, gray_image;
	Mat laplacian, sharpened_32F, sharpened;
	namedWindow("sharpened gray", WINDOW_AUTOSIZE);
	namedWindow("original gray", WINDOW_AUTOSIZE);
	float multiplier = 0.3;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	gray_image.convertTo(gray_image_32F, CV_32FC1);
	Laplacian(gray_image_32F, laplacian, CV_32F, 3);
	sharpened_32F = gray_image_32F - multiplier*laplacian;
	sharpened_32F.convertTo( sharpened, CV_8UC1);
	imshow("sharpened gray", sharpened);
	imshow("original gray", gray_image);
	waitKey(0);
}

//git commit -m "sharpening image by subtracting laplacian*X from the original image"

