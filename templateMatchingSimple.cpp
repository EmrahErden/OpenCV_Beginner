//g++ -std=c++11 templateMatchingSimple.cpp `pkg-config --libs opencv` -o templateMatchingSimple

/*
Template matching has been used quite widely; for example, in stereo vision (where we
are trying to determine how far away objects are on the basis of a stereo pair of images)
correspondences are sought from one image to the other using very small sub-images; and in
tracking of moving objects, where we can typically assume that the appearance of an object
being tracked will change slowly from frame to frame and again can use small templates on
the moving object.

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
	if(!cap.isOpened())
	{
		cerr << "Couldn't open the capture!\n";
	}
	while(frameCounter < 30)
	{
		//grabbing some frames
		cap >> frame;
		imshow("Video", frame);
		frameCounter++;
		waitKey(10);
	}
	destroyWindow("Video");
	cvtColor(frame, frame, CV_BGR2GRAY);

	//Create a window
	namedWindow("ImageDisplay", 1);

	//set the callback function for any mouse event
	setMouseCallback("ImageDisplay", CallBackFunc, NULL);

	//show the last frame
	imshow("ImageDisplay", frame);

	// Wait until user is ready after 2 corner/input points
	waitKey(0);
	destroyWindow("ImageDisplay");

	int Iwidth = inputPoints[1].x - inputPoints[0].x;
	int Iheight = inputPoints[1].y - inputPoints[0].y;
	cout << "Iwidth: " << Iwidth << "\n";
	cout << "Iheight: " << Iheight << "\n";

	Rect roi(inputPoints[0].x, inputPoints[0].y, Iwidth, Iheight);
	//creating the search image, the region of interest
	Mat template_image = frame(roi);
	imshow("template_image", template_image);
	waitKey(0);
	Mat search_image = frame;

	//now we are ready to search the video stream
	Mat matching_space;
	matching_space.create(search_image.cols-template_image.cols+1, search_image.rows-template_image.rows+1, CV_32FC1);
	//http://docs.opencv.org/doc/tutorials/imgproc/histograms/template_matching/template_matching.html
	matchTemplate( search_image, template_image, matching_space, CV_TM_SQDIFF_NORMED);
	
	Mat matching_space_8bit, thresholded_matching_space_8bit;

	matching_space.convertTo(matching_space_8bit, CV_8UC1, 255);

	//http://answers.opencv.org/question/27248/max-and-min-values-in-a-mat/
	double minVal, maxVal; 
	Point minLoc, maxLoc, updated_minLoc;
	minMaxLoc(matching_space_8bit, &minVal, &maxVal, &minLoc, &maxLoc);

	cout << "matching_space min and max values: "<< minVal << ", " << maxVal << "\n";
	cout << "matching_space min value, x & y positions: "<< minLoc.x << ", " << minLoc.y << "\n";

	updated_minLoc.x = minLoc.x + Iwidth/2;
	updated_minLoc.y = minLoc.y + Iheight/2;
	int r = max(Iwidth, Iheight)/2;

	Scalar color = Scalar(255);
	circle(frame, updated_minLoc, r, color, 2, 8, 0 );

	imshow("updated frame",frame);
	waitKey(0);
}
