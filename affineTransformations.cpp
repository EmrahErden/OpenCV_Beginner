//g++ affineTransformations.cpp `pkg-config --libs opencv` -o affineTransformations

/*
In OpenCV, linear transformations (such as the affine transformation) are 
described using floating point two-dimensional matrices such as:
Mat affine_matrix = Mat::eye( 2, 3, CV_32FC1 );
in this case with two rows and three columns.
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

int main(void)
{
	Mat img = imread("sample.jpeg"), gray, result;
	cvtColor(img, gray, CV_BGR2GRAY);
	namedWindow("affine", WINDOW_AUTOSIZE);
	Mat affine_matrix = Mat::eye( 2, 3, CV_32FC1 );
	//modifying affine_matrix[0][1], remember .data is pointer
	*(((float*)(affine_matrix.data))+1) = 0.17; //Skewing by angle φ, 0.17=tan(φ).
	//modifying affine_matrix[0][2]
	*(((float*)(affine_matrix.data))+2) = -10;	//translating to the left by X pixels
	//doing the affine transform
	warpAffine(gray, result, affine_matrix, gray.size() );
	imshow("affine", result);
	waitKey(0);
}

