//g++ simpleImageRead.cpp `pkg-config --libs opencv` -o simpleImageRead

//#include <opencv2/opencv.hpp> //Include file for every supported OpenCV function
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;

int main(void) 
{
	cv::Mat img = cv::imread("sample.jpeg",-1);
	if( img.empty() )
	{
		cout << "NO IMAGE FILE!\n";
	}
	cv::namedWindow( "Example1", cv::WINDOW_AUTOSIZE );
	cv::imshow( "Example1", img );
	cv::waitKey( 0 );
	cv::destroyWindow( "Example1" );
}
