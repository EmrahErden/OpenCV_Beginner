//g++ colorInversion.cpp `pkg-config --libs opencv` -o colorInversion

/*
The most common representation for colour images is to use three channels correspond-
ing, roughly, to the red (700 nm), green (546.1 nm) and blue (435.8nm) wavelengths
(see Figure 2.5). What this means in effect is that the photosensitive elements in the
camera are spectrally sensitive to wavelengths which are centred on those colours

dawson-howe
*/

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;

void invertColor(const Mat & input, Mat & output)
{
	CV_Assert(input.type() == CV_8UC3);
	CV_Assert(output.type() == CV_8UC3);

	for(int row=0; row < input.rows; row++)
	{
		for(int col=0; col < input.cols; col++)
		{
			for(int chn = 0; chn < 3; chn++)
			{
				output.at<Vec3b>(row,col)[chn] = 255 - 
					input.at<Vec3b>(row,col)[chn];
			}
		}
	}
}

int main(void)
{
	Mat input = imread("sample.jpeg");
	Mat output = input.clone(); //If you want "new" full copy of the matrix
	//http://stackoverflow.com/questions/21659496/deep-copy-of-opencv-cvmat

	namedWindow("Input", WINDOW_AUTOSIZE);
	namedWindow("Output", WINDOW_AUTOSIZE);	
	invertColor(input, output);
	imshow("Input", input);
	imshow("Output", output);	
	waitKey(0);
}

/*
RGB colour information can easily be converted to grey-scale using a formula such as:
Y = 0.299R + 0.587G + 0.114B
dawson-howe
*/
