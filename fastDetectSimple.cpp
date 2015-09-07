//g++ -std=c++11 fastDetectSimple.cpp `pkg-config --libs opencv` -o fastDetectSimple

/*
Given an edge image, it is often impossible to determine locally the movement of the edge
from one frame to the next frame in an image sequence (see Figure 7.1). This is caused to a
large extent by the ambiguity of the match

To overcome this problem, a common approach in computer vision is to instead make use
of corners, image features or interest points

Most algorithms for corner detection follow a similar serious of steps, which are presented
here:
1) Determine cornerness values.
2) Non-maxima suppression. Suppress all cornerness values which are less than a local neigh-
bour (within n pixels distance; e.g. n = 3) in order to avoid multiple responses for the same
corner.
3) Threshold the cornerness map. Finally, we need to select those cornerness values which
remain and are significant and this is typically done using thresholding

(FAST detector is a very special corner detector) 
The FAST detector, as its name suggests, is much faster than the other detectors.
it does does not compute a cornerness metric. Instead it looks at a circle of points (normally of radius 3,
mapping to 16 pixels) around the current pixel being considered. The current pixel p is a corner
if there is a contiguous arc of at least 9 pixels around the circle which are all brighter or all darker
than the nucleus (the current point or an average of the current point and its close neighbours)
by some predefined threshold t. The value 9 is chosen to exclude edges and has been shown
to give the best results, although it should be clear that some false responses will be generated
at such a level. In fact a value of 12 was suggested in the original work.

dawson-howe
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp" //FeatureDetector
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(void)
{
	Mat image = imread("licencePlate.jpg", IMREAD_GRAYSCALE);

	vector<KeyPoint> kpt1;
	Mat desc1;

	Ptr<AKAZE> akaze = AKAZE::create();
	akaze->detectAndCompute(image, noArray(), kpt1, desc1);

	//-- Draw keypoints
	Mat image_kpt1;
	drawKeypoints(image, kpt1, image_kpt1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

	//-- Show detected (drawn) keypoints
	imshow("Keypoints 1", image_kpt1);
	waitKey(0);

	//FAST example in dawson-howe was not working.
	//this AKAZE detector example is taken from:
	//http://docs.opencv.org/3.0-beta/doc/tutorials/features2d/akaze_matching/akaze_matching.html#akaze
	//how to draw key points:
	//http://docs.opencv.org/3.0-beta/doc/tutorials/features2d/feature_detection/feature_detection.html#feature-detection
}
