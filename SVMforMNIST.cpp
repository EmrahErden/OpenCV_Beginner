//g++ -std=c++11 SVMforMNIST.cpp `pkg-config --libs opencv` -o SVMforMNIST

/*
1st:
Reading the out MNIST DATABASE data set from binary files:
http://yann.lecun.com/exdb/mnist/

2nd:
After saving the data into cv::Mat files, samples are visualy tested via testDisplay function

WARNING:
We call the method CvSVM::train to build the SVM model. Watch out that the training process may take a quite long time. Have patiance when your run the program.
http://docs.opencv.org/doc/tutorials/ml/non_linear_svms/non_linear_svms.html

We will be only using first 1000 training examples only...
*/

#include <iostream>
#include <fstream>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>

#define IMAGE_SIZE 28
#define MINST_OFFSET_DF 16 //offset in data file in terms of bytes
#define MINST_OFFSET_LF 8 //offset in labels file in terms of bytes
#define NUM_TRAINDATA 1000 //number of examples in the training data set
#define NUM_TESTDATA 1000 //number of examples in the test data set

using namespace std;
using namespace cv;
using namespace cv::ml;

void extractBinaryDataFile(Mat& Data, string DataFileName, int numExamples)
{
  	streampos size;

	//reading the data file
	char * memblock;
  	ifstream file (DataFileName, ios::in|ios::binary|ios::ate);
  	if (file.is_open())
  	{
    		size = file.tellg();
    		memblock = new char [size];
    		file.seekg (0, ios::beg);
    		file.read (memblock, size);
    		file.close();

    		cout << "\nSuccessfully read the data file...\n\n";

		uchar* pData = Data.ptr<uchar>(0,0);
		int numBytesinData = IMAGE_SIZE*IMAGE_SIZE*numExamples;

		for(int i=0; i<numBytesinData; i++)
		{
			*pData = memblock[MINST_OFFSET_DF+i];
			pData++;
		}
		

    		delete[] memblock;
  	}
  	else cout << "\nUnable to open the data file!!\n\n";	
}

void extractBinaryLabelsFile(Mat& Labels, string LabelsFileName, int numExamples)
{
  	streampos size;

	//reading the labels file
	char * memblock;
  	ifstream file (LabelsFileName, ios::in|ios::binary|ios::ate);
  	if (file.is_open())
  	{
    		size = file.tellg();
    		memblock = new char [size];
    		file.seekg (0, ios::beg);
    		file.read (memblock, size);
    		file.close();

    		cout << "\nSuccessfully read the labels file...\n\n";

		uchar* pLabels = Labels.ptr<uchar>(0,0);
		int numBytesinData = numExamples;

		for(int i=0; i<numBytesinData; i++)
		{
			*pLabels = memblock[MINST_OFFSET_LF+i];
			pLabels++;
		}
		
    		delete[] memblock;
  	}
  	else cout << "\nUnable to open the labels file!!\n\n";	
}

void testDisplay(Mat& Data, Mat& Labels, int displayLoc)
{
	Mat displayImage(IMAGE_SIZE, IMAGE_SIZE, CV_8UC1);
	uchar* pDisplayImage = displayImage.ptr<uchar>(0,0);
	uchar* pData = Data.ptr<uchar>(0,0);

	for (int i=0; i<IMAGE_SIZE*IMAGE_SIZE; i++)
	{
		*pDisplayImage = *(pData+IMAGE_SIZE*IMAGE_SIZE*displayLoc+i);
		pDisplayImage++;
	}
	imshow("example display", displayImage);
	cout << "Displayed image has value of : " << (int)Labels.at<uchar>(displayLoc,0) <<"\n\n"; 
	waitKey(0);
}

//Error calculation after training
float errorCheck(Ptr<SVM> svm, Mat& Data, Mat& Labels)
{
	int numVariables = IMAGE_SIZE*IMAGE_SIZE;

	Mat sampleMat2(1, numVariables, CV_32FC1);
	int correctCount = 0;
	int wrongCount = 0;

	for(int j=0; j<Data.rows; j++)
	{
		for(int i = 0; i < numVariables; i++)
		{
			sampleMat2.at<float>(0,i) = Data.at<float>(j,i);
		}

		int response = svm->predict(sampleMat2);
		//cout << Labels.at<int>(j,0) << " vs " << response << "\n";
		(response==Labels.at<int>(j,0)) ? correctCount++ : wrongCount++;
	}	
	
	return (float)wrongCount/(float)(correctCount+wrongCount);
}

int main(void)
{
	Mat trainData(NUM_TRAINDATA, IMAGE_SIZE*IMAGE_SIZE, CV_8UC1);
	Mat trainLabels(NUM_TRAINDATA, 1, CV_8UC1);
	Mat trainData_32F(NUM_TRAINDATA, IMAGE_SIZE*IMAGE_SIZE, CV_32FC1);
	Mat trainLabels_32S(NUM_TRAINDATA, 1, CV_32SC1);

	Mat testData(NUM_TESTDATA, IMAGE_SIZE*IMAGE_SIZE, CV_8UC1);
	Mat testLabels(NUM_TESTDATA, 1, CV_8UC1);
	Mat testData_32F(NUM_TESTDATA, IMAGE_SIZE*IMAGE_SIZE, CV_32FC1);
	Mat testLabels_32S(NUM_TESTDATA, 1, CV_32SC1);

	//extracting raw data from binary training files
	extractBinaryDataFile(trainData, "train-images-idx3-ubyte", NUM_TRAINDATA);
	extractBinaryLabelsFile(trainLabels, "train-labels.idx1-ubyte", NUM_TRAINDATA);

	extractBinaryDataFile(testData, "t10k-images.idx3-ubyte", NUM_TESTDATA);
	extractBinaryLabelsFile(testLabels, "t10k-labels.idx1-ubyte", NUM_TESTDATA);

	//visualy inspecting a random sample
	srand(time(0));
	testDisplay(trainData, trainLabels, rand()%NUM_TRAINDATA);

	//after testing the Mat variables correctness, it is now time to convert them to apropriate data types for SVM train
	trainData.convertTo(trainData_32F, CV_32FC1);
	trainLabels.convertTo(trainLabels_32S, CV_32SC1);

	testData.convertTo(testData_32F, CV_32FC1);
	testLabels.convertTo(testLabels_32S, CV_32SC1);

	//set up and Train the support vector machines parameters 

	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setC(0.1);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, (int)1e7, 1e-6));
	cout << "SVM training in progress...\n";
	svm->train(trainData_32F, ROW_SAMPLE, trainLabels_32S);
	cout << "Finished training...\n";
	float Ein = errorCheck(svm, trainData_32F, trainLabels_32S);
	cout << "Ein : " << Ein << "\n";
	float Eout = errorCheck(svm, testData_32F, testLabels_32S);
	cout << "Eout : " << Eout << "\n";
	cout << "--------------------------------\n\n";

}

