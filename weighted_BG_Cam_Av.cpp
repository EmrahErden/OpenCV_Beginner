//g++ -std=c++11 weighted_BG_Cam_Av.cpp `pkg-config --libs opencv` -o weighted_BG_Cam_Av

/*
Not a very efficient conversion, since there is too much conversion in between
CV_8U and CV_32F
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	VideoCapture cap;
	cap.open(0);
	Mat frame;
	vector<Mat> rgbChannels(3);
	Mat blue, blue32;
	namedWindow("averaged scene - blue channel", WINDOW_AUTOSIZE);
	
	cap >> frame;
	split(frame, rgbChannels);
	blue = rgbChannels[0];
	Mat running_average_background_blue_channel_32 = cv::Mat::zeros(blue.size(), CV_32FC1);
	Mat running_average_background_blue_channel;
	while(true)
	{
		cap >> frame;
		cout << "frame size: " << frame.size() << "\n";
		split(frame, rgbChannels);
		blue = rgbChannels[0];
		blue.convertTo(blue32, CV_32FC1);

		cout << "blue32 size: " << blue32.size() << "\n";
		cout << "running_average_background_blue_channel size: " << running_average_background_blue_channel_32.size() << "\n";

		accumulateWeighted(blue32, running_average_background_blue_channel_32, 0.01 );
		running_average_background_blue_channel_32.convertTo(running_average_background_blue_channel, CV_8UC1);
		imshow("averaged scene - blue channel", running_average_background_blue_channel);
		waitKey(40);
	}

}

