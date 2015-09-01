//g++ humanSkinDetect.cpp `pkg-config --libs opencv` -o humanSkinDetect

/*
Skin detection can be performed by simply analysing pixels values. Through simple experi-
mentation it was found that:
etc etc
dawson-howe
*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;

void skinDetector(Mat& bgr_input, Mat& hls_input, Mat & bgr_output)
{
	uchar H, L, S;
	double LS_ratio;
	bool skinDetect;
	int numCols = bgr_input.cols;
	int numRows = bgr_input.rows;

	uchar* pbgr_input = bgr_input.ptr<uchar>(0,0);
	uchar* phls_input = hls_input.ptr<uchar>(0,0);
	uchar* pbgr_output = bgr_output.ptr<uchar>(0,0);

	for(int row = 0; row< numRows; row++)
	{
		for(int cols = 0; cols<numCols; cols++)
		{
			H = *(phls_input);
			L = *(phls_input+1);
			S = *(phls_input+2);
			LS_ratio = ((double) L) / ((double) S);
			skinDetect = ((S >= 50) && (LS_ratio > 0.5) && (LS_ratio < 3.0) && ((H <= 14)  || (H >= 165)));
			if(skinDetect)
			{
				*(pbgr_output) = *(pbgr_input);
				*(pbgr_output+1) = *(pbgr_input+1);
				*(pbgr_output+2) = *(pbgr_input+2);
			}else
			{
				*(pbgr_output) = 0;
				*(pbgr_output+1) = 0;
				*(pbgr_output+2) = 0;
			}

			phls_input += 3;
			pbgr_input += 3;
			pbgr_output += 3;
		}
	}
}

int main(void)
{
	Mat bgr_input;
	Mat hls_input;
	Mat bgr_output(480, 640, CV_8UC3);
	namedWindow("video", WINDOW_AUTOSIZE);
	VideoCapture cap;
	cap.open(0);

	while(1)
	{
		cap >> bgr_input;
		//std::cout << bgr_input.cols << " " << bgr_input.rows << "\n";
		cvtColor(bgr_input, hls_input, CV_BGR2HLS);
		skinDetector(bgr_input, hls_input, bgr_output);
		imshow("video", bgr_output);
		waitKey(40);
	}

	//Code will not work since bgr to hls conversion by OpenCV is quite faulty.
}



/*
int main(void)
{
	Mat bgr_input = imread("humans2.jpg");
	Mat hls_input;
	Mat bgr_output = bgr_input.clone();
	namedWindow("only skins", WINDOW_AUTOSIZE);
	cvtColor(bgr_input, hls_input, CV_BGR2HLS);
	skinDetector(bgr_input, hls_input, bgr_output);
	imshow("only skins", bgr_output);
	waitKey(0);
}
*/
