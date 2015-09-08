//g++ -std=c++11 templateMatchingExtended.cpp `pkg-config --libs opencv` -o templateMatchingExtended

/*
WARNING:
One problem that template matching has is that it is very dependent on the appearance of the
object of interest being almost the same (ignoring overall lumination) in the template as in
the search image. Any distortion of this appearance can have quite a significant impact on the
matching score obtained.

dawson-howe
*/

/*
A very good comment on grayscale capture:
http://answers.opencv.org/question/46371/capture-in-greyscale/
WHY WE SHOULD USE THE ORIGINAL CAPTURED CAMERA FRAMES !!!
*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace cv;
using namespace std;

#define Mult 3 //multiplier for search region size.

int match_method;
vector<Point2f> inputPoints(2); //global variables storing the mouse clicks
int counter = 0;
int frameCounter = 0;
int thresholdVal = 10;

//the function where we select the region of interest via left mouse clicks
//2 points required
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        Point2f pt(x,y);
        inputPoints[counter] = pt;
        counter++;
        cout << "Point # " << counter << " location: " << x << ", " << y << "\n";
    }  
}

int main(void)
{
	//starting video capture
	VideoCapture cap;
	Mat frame;
	namedWindow("Video", WINDOW_AUTOSIZE);

	cap.open(0);
	if(!cap.isOpened())//error if unable to capture
	{
		cerr << "Couldn't open the capture!\n";
	}
	while(counter != 2)
	{
		//grabbing some frames
		cap >> frame;
		imshow("Video", frame);
		frameCounter++;
		setMouseCallback("Video", CallBackFunc, NULL);
		waitKey(10);
	}//exit if 2 corners are selected

	//after 2 mouse clicks, we press enter and last frame is split into seperate color channels
	//we will be dealing with blue channel (channel #0) of the BGR image
	vector<Mat> rgbChannels(3);
	Mat blue;
    	split(frame, rgbChannels);
	blue = rgbChannels[0];

	//calculating the width and height of the selected region
	int Iwidth = inputPoints[1].x - inputPoints[0].x;
	int Iheight = inputPoints[1].y - inputPoints[0].y;
	cout << "Iwidth: " << Iwidth << "\n";
	cout << "Iheight: " << Iheight << "\n";

	Rect roi(inputPoints[0].x, inputPoints[0].y, Iwidth, Iheight);
	//creating the search image, the region of interest, in blue channel
	Mat template_image_shallow = blue(roi);
	Mat template_image = template_image_shallow.clone();
	imshow("template_image", template_image);
	waitKey(0);
	destroyWindow("template_image");

	Mat search_image = blue;//we will not be searching every pixel

	//now we are ready to search the "entire" stream frame
	Mat matching_space;
	//matching_space.create(search_image.cols-template_image.cols+1, search_image.rows-template_image.rows+1, CV_32FC1);
	matching_space.create(Iwidth*Mult-template_image.cols+1, Iheight*Mult-template_image.rows+1, CV_32FC1);
	//http://docs.opencv.org/doc/tutorials/imgproc/histograms/template_matching/template_matching.html

	int r = max(Iwidth, Iheight)/2;

	double minVal, maxVal; 
	Point minLoc, maxLoc, updated_minLoc;
	//initializing minLoc
	minLoc.x = inputPoints[0].x;
	minLoc.y = inputPoints[0].y;

	while(1)
	{
		cap >> frame;
		split(frame, rgbChannels);
		search_image = rgbChannels[0];

		//making match with the master copy
		matchTemplate(search_image, template_image, matching_space, CV_TM_SQDIFF_NORMED);
		minMaxLoc(matching_space, &minVal, &maxVal, &minLoc, &maxLoc);

		updated_minLoc.x = minLoc.x + Iwidth/2;
		updated_minLoc.y = minLoc.y + Iheight/2;

		Scalar color = Scalar(0,0,255);
		circle(frame, updated_minLoc, r, color, 2, 8, 0 );

		imshow("updated frame", frame);
		waitKey(40);
	}

}
