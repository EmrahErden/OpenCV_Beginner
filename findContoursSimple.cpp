//g++ -std=c++11 findContoursSimple.cpp `pkg-config --libs opencv` -o findContoursSimple

/*
In OpenCV, each individual contour is stored as a vector of points, and all the contours
are stored as a vector of contours (i.e. a vector of vectors of points).
dawson-howe
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace cv;
using namespace std;

RNG rng(12345);

int main(void)
{
	Mat img = imread("otsuBinaryThreshold.png"), gray, binary;
	cvtColor(img, gray, CV_BGR2GRAY);
	int thresholdVal = 30; //has no use if THRESH_OTSU is used
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//thresholding for finding the contours, remember it is applied to grayscale images
	threshold(gray, binary, thresholdVal, 255, THRESH_BINARY | THRESH_OTSU );	
	findContours( binary, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE );

  	/// It is not possible to display the contours directly. To draw contours drawContours command is used
  	Mat drawing = Mat::zeros(img.size(), CV_8UC3 );
  	for( int i = 0; i< contours.size(); i++ )
     	{
       		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     	}

  	/// Show in a window
  	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  	imshow( "Contours", drawing );
  	imshow( "Original", img );
	waitKey(0);
}
