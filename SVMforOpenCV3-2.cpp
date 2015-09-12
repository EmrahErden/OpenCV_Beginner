//g++ -std=c++11 SVMforOpenCV3-2.cpp `pkg-config --libs opencv` -o SVMforOpenCV3-2

/*
Support Vector Machines for Non-Linearly Separable Data
modified:
http://docs.opencv.org/master/d0/dcc/tutorial_non_linear_svms.html#gsc.tab=0
for non-seperable data

looks like 
*/

#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/imgproc.hpp>

#define NTRAINING_SAMPLES   1000         // Number of training samples in total
#define FRAC_LINEAR_SEP     0.95f        // Fraction of samples which compose the linear separable part, this value is approximate
#define numTrainingPoints 900
#define numVariables 2
#define thickness -1
#define lineType 8
#define lowerLimit -1.
#define upperLimit 1.
#define numXorYGrids 512
			 
using namespace std;
using namespace cv;
using namespace cv::ml;

//this function is taken from file SVMforOpenCV3.cpp
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

//this function is taken from file SVMforOpenCV3.cpp
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

//modify the labels in a wrong way, so that the data is non-seperable
void markWrongly(Mat& labels, int numWrong)
{
	int rnd;
	int currentLabel;
	RNG rng(100); // Random value generation class

	for(int i = 0; i< numWrong; i++)
	{
		rnd = rng.uniform(0, NTRAINING_SAMPLES);
		labels.at<int>(rnd,0) *= (-1);//change the sign of the label		
	}
}

//showing decision regions and modifying the image file accordingly
void showDecisionRegions(Ptr<SVM> svm, Mat& image)
{
	Vec3b green(0,255,0), red(0,0,255);
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
			{
				image.at<Vec3b>(j, i)  = green;
			}
		    	else 
			{			 	
				image.at<Vec3b>(j, i)  = red;
			}
		}
	}
	imshow("decision regions", image);
	waitKey(0);
}

//marking the data points into the plot
void addPointsIntoPlot(Ptr<SVM> svm, Mat& image, Mat& input, Mat& inputLabel)
{	
	Mat sampleMat2(1, numVariables, CV_32FC1);
	int correctCount = 0;
	int wrongCount = 0;

	for(int j=0; j<input.rows; j++)
	{
		for(int i = 0; i < numVariables; ++i)
		{
			sampleMat2.at<float>(0,i) = input.at<float>(j,i);
		}
		std::cout << sampleMat2 << "\n";

		int response = svm->predict(sampleMat2);
		int y = (int)((sampleMat2.at<float>(0,0)-lowerLimit)/(upperLimit-lowerLimit)*numXorYGrids);
		int x = (int)((sampleMat2.at<float>(0,1)-lowerLimit)/(upperLimit-lowerLimit)*numXorYGrids);
		std::cout << "y: " << y << ", x: " << x << "\n";
		if(response==inputLabel.at<int>(j,0))
		{
			circle(image,  Point(y, x), 3, Scalar(0, 0, 0), thickness, lineType);
			correctCount++;
		}else
		{
			putText(image, "x", Point(y, x), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,0), 2);
			wrongCount++;
		}
		
	}

	cout << "Correctly marked training points after prediction : " << correctCount << "\n";
	cout << "Wrongly marked training points after prediction : " << wrongCount << "\n";
	cout << "Accuracy : %" << (float)correctCount/(float)(wrongCount+correctCount)*100. << "\n";

	imshow("marking the data", image);
	waitKey(0);
}

int main(void)
{
	//--------------------- 1. Set up training data randomly ---------------------------------------
	Mat trainData(NTRAINING_SAMPLES, numVariables, CV_32FC1);
	Mat labels   (NTRAINING_SAMPLES, 1, CV_32SC1);
	cv :: randu(trainData, (float)lowerLimit, (float)upperLimit) ;
	//the Mat file trainData is now filled with uniformly	
	//distributed random numbers between lowerLimit and upperLimit.

    	// Find out how many data points are wrong
    	int numWrong = (int) ((1.0-FRAC_LINEAR_SEP) * NTRAINING_SAMPLES);
	//correctly label the data at first
	labelData(trainData, labels, 0);
	//then mark the wrong data points
	markWrongly(labels, numWrong);

	cout << "numWrong : " << numWrong << "\n";
	cout << "training Data : \n" << trainData << "\n";
	cout << "labels : \n" << labels << "\n";


	//------------------------ 2. Set up and Traing the support vector machines parameters --------------------
	cout << "Starting training process" << endl;
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setC(0.1);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, (int)1e7, 1e-6));
	svm->train(trainData, ROW_SAMPLE, labels);
	cout << "Finished training process" << endl;

	//showing the decision regions after the training
	Mat image = Mat::zeros(numXorYGrids, numXorYGrids, CV_8UC3);//display Mat image
	showDecisionRegions(svm, image);

	//marking training data into plot
	addPointsIntoPlot(svm, image, trainData, labels);
}

