//g++ bewareAtOpenCV.cpp `pkg-config --libs opencv` -o bewareAtOpenCV

/*
In OpenCV, to efficiently process images we must avoid use of the at function (as this adds
significant computational costs). Instead, we must use pointers to the image data within
the Mat structures. Unfortunately, this produces code that is hard to understand.
dawson-howe
*/

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;

void invert_Via_Pointers_and_BitShift(Mat& input, Mat& output)
{
	int numCols = input.cols;
	int numRows = input.rows;

	uchar* pInput = input.ptr<uchar>(0,0);
	uchar* pOutput = output.ptr<uchar>(0,0);

	for(int row = 0; row< numRows; row++)
	{
		for(int cols = 0; cols<numCols; cols++)
		{
			for(int chn = 0; chn < 3; chn++)
			{
/*
Bitwise Exclusive-Or (XOR)
http://www.cprogramming.com/tutorial/bitwise_operators.html
*/
				*pOutput = *pInput ^ 0xFF;
				pInput++;
				pOutput++;
			}
		}
	}
}

int main(void)
{
	Mat img = imread("sample.jpeg");
	Mat output = img.clone();
	namedWindow("original", WINDOW_AUTOSIZE);
	namedWindow("modified", WINDOW_AUTOSIZE);
	invert_Via_Pointers_and_BitShift(img, output);
	imshow("original",img);
	imshow("modified",output);
	waitKey(0);
}
