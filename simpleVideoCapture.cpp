//g++ simpleVideoCapture.cpp `pkg-config --libs opencv` -o simpleVideoCapture

//video capture
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main(void)
{
	namedWindow("Video", WINDOW_NORMAL);
	VideoCapture cap;
	Mat frame;
	cap.open("sampleVideo.divx");

	while(1)
	{
		cap >> frame;
		if(!frame.data)
		{
			break;
		}
		imshow("Video", frame);
		waitKey(0);
	}
}
