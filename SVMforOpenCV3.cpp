//g++ -std=c++11 SVMforOpenCV3.cpp `pkg-config --libs opencv` -o SVMforOpenCV3

/*
modified 
http://www.bytefish.de/pdf/machinelearning.pdf (this is abit old)
and
http://docs.opencv.org/master/d1/d73/tutorial_introduction_to_svm.html#gsc.tab=0
(newer version)
*/

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ml.hpp"
#include <iostream>

#define numTrainingPoints 900
#define numVariables 2
#define thickness -1
#define lineType 8
#define lowerLimit -1.
#define upperLimit 1.
#define numXorYGrids 512

using namespace cv;
using namespace cv::ml;

int labelFunct(const float x, const float y, const int func)
{
	switch(func)
	{
		//labeling data set x+y>0 (as 1), or x+y<0 (as -1)
		case 0:
			return (x+y>=0.) ? 1 : -1; //Ternary conditional
			break;
		case 1:
			return (x*y>=0.) ? 1 : -1;
			break;
	}
}

void labelData(Mat& trainingData, Mat& labeledData, int func)
{
	int numDataPoints = trainingData.rows;
	cv::Mat result(trainingData.rows, trainingData.cols, CV_32FC1);
	float* pTrainingData = trainingData.ptr<float>(0,0);
	int* plabeledData = labeledData.ptr<int>(0,0);
	for(int i = 0; i<numDataPoints; i++)
	{
		float x = *pTrainingData;
		float y = *(++pTrainingData);
		*plabeledData = labelFunct(x, y, func);
		++pTrainingData;
		++plabeledData;	
	}	
}

int main(void)
{
	cv :: Mat trainingData(numTrainingPoints, numVariables, CV_32FC1 );
	cv :: Mat labeledData(numTrainingPoints, 1, CV_32SC1);
	cv :: randu(trainingData, (float)lowerLimit, (float)upperLimit) ;
	//the Mat file trainingData is now filled with uniformly	
	//distributed random numbers between lowerLimit and upperLimit.
	labelData(trainingData, labeledData, 0);
	
	/*
	//nicely displaying the Mat files
	//thanks to http://stackoverflow.com/questions/7970988/print-out-the-values-of-a-mat-matrix-in-opencv-c
	std::cout << "trainingData = "<< std::endl << " "  << trainingData << std::endl;
	std::cout << "labeledData = "<< std::endl << " "  << labeledData << std::endl;
	*/

	// Train the SVM
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-6));
	svm->train(trainingData, ROW_SAMPLE, labeledData);

	Mat image = Mat::zeros(numXorYGrids, numXorYGrids, CV_8UC3);
	Vec3b green(0,255,0), red (0,0,255);
	Mat sampleMat(1, numVariables, CV_32FC1);
	float* pSampleMat = sampleMat.ptr<float>(0,0);

	//Show the decision regions given by the SVM
	for (int j = 0; j < numXorYGrids; ++j)
	{
		for (int i = 0; i < numXorYGrids; ++i)
		{
			*pSampleMat = (upperLimit-lowerLimit)*(float)j/(float)numXorYGrids+lowerLimit;
			//std::cout << "j: " << j << ", i: " << i << ", 1st : " << *pSampleMat;
			*(++pSampleMat) = (upperLimit-lowerLimit)*(float)i/(float)numXorYGrids+lowerLimit;
			//std::cout << ", 2nd: " << *pSampleMat << "\n";
			--pSampleMat;

		    	int response = svm->predict(sampleMat);
			//std::cout << "response : " << response << "\n\n";
		    	if (response == 1)
				image.at<Vec3b>(j, i)  = green;
		    	else 
			 	image.at<Vec3b>(j, i)  = red;
		}
	}

	//showing the training data in the plot
	Mat sampleMat2(1, numVariables, CV_32FC1);
	for(int j=0; j<numTrainingPoints; j++)
	{
		for(int i = 0; i < numVariables; ++i)
		{
			sampleMat2.at<float>(0,i) = trainingData.at<float>(j,i);
		}
		std::cout << sampleMat2 << "\n";

		int response = svm->predict(sampleMat2);
		int y = (int)((sampleMat2.at<float>(0,0)-lowerLimit)/(upperLimit-lowerLimit)*numXorYGrids);
		int x = (int)((sampleMat2.at<float>(0,1)-lowerLimit)/(upperLimit-lowerLimit)*numXorYGrids);
		std::cout << "y: " << y << ", x: " << x << "\n";
		if(response==labeledData.at<int>(j,0))
		{

			circle( image,  Point(y, x), 3, Scalar(0, 0, 0), thickness, lineType);
		}else
		{
			putText(image, "x", Point(y, x), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
		}
		
	}

	imshow("image", image);
	waitKey(0);	
}
	
