//g++ randomNumberSimple2.cpp `pkg-config --libs opencv` -o randomNumberSimple2

//modified http://www.bytefish.de/pdf/machinelearning.pdf
//random noise image display

/*
	Uniform Distributions can be generated in OpenCV with the help of cv::randu. The signature of
	cv::randu is:
	void randu ( Mat & mtx , const Scalar & low , const Scalar & high );
*/

#include "opencv2/highgui/highgui.hpp"

#define numTrainingPoints 480
#define numVariables 640

int main(void)
{
	cv :: Mat randomNoise(numTrainingPoints, numVariables, CV_32FC1 );
	cv :: randu (randomNoise, 0., 1.) ;
	//the Mat file randomNoise is now filled with uniformly	
	//distributed random numbers between 0 and 1.
	cv::imshow("random noise", randomNoise);
	cv::waitKey(0);
}
