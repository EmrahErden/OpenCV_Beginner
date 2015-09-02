//g++ simpleHistogram.cpp `pkg-config --libs opencv` -o simpleHistogram

/*
Modified: http://docs.opencv.org/3.0-beta/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
for single channel images
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

//histogram display for gray-scale images
void computeGSHist(Mat & input)
{
  	/// Establish the number of bins
  	int histSize = 256;

  	/// Set the ranges ( for B,G,R) )
  	float range[] = { 0, 256 } ;
  	const float* histRange = { range };

  	bool uniform = true; bool accumulate = false;

  	Mat hist;

  	/// Compute the histograms:
  	calcHist(&input, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC1, Scalar(0) );

	/// Normalize the result
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

 	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
		Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ), Scalar( 255));
	}

	/// Display
	namedWindow("calcHist Demo", WINDOW_AUTOSIZE );
	imshow("calcHist Demo", histImage );

	waitKey(0);
}

int main(void)
{
	Mat img = imread("sample.jpeg");
	Mat imgGS, hist;
	cvtColor(img, imgGS, COLOR_BGR2GRAY);
	computeGSHist(imgGS);
}

