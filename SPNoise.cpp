//g++ SPNoise.cpp `pkg-config --libs opencv` -o SPNoise

#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;

void SPNoise(Mat & input, Mat & output, int oneInX)
{
	output = input.clone();
	uchar *pOutput = output.ptr<uchar>(0,0);
	int numChn = input.channels();
	int numRows = input.rows;
	int numCols = input.cols;

	for(int rows = 0; rows < numRows; rows++)
	{
		for(int cols = 0; cols < numCols ; cols++)
		{
			for(int chn = 0; chn < numChn ; chn++)
			{
				if(rand()%oneInX==0)
				{
					if(rand()%2==0)
					{
						*pOutput = 0;
					}else
					{
						*pOutput = 255;
					}
				}

				pOutput++;
			}
		}
	}
}

int main(void)
{
	Mat img = imread("sample.jpeg");
	Mat noisyImg;
	namedWindow("Salt and Pepper Noised Image", WINDOW_AUTOSIZE);
	SPNoise(img, noisyImg, 20);
	imshow("Salt and Pepper Noised Image", noisyImg);
	std::cout << rand() << "\n";
	waitKey(0);
}
