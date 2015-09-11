//g++ randomNumberSimple.cpp `pkg-config --libs opencv` -o randomNumberSimple

//http://opencvexamples.blogspot.com/2013/10/random-number-generator-drawing.html
/*
class RNG - Random number generator.

*/

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

int main(void)
{
	cv::RNG rnum;
	int n = 10;
	double rnd;

	for(int i = 1; i< n; i++)
	{
		rnd = rnum.uniform((double)0, (double)1);
		std::cout << " i = " << i << ", rnd = " << rnd << "\n";
	}
}
