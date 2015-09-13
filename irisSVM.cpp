//g++ -std=c++11 irisSVM.cpp `pkg-config --libs opencv` -o irisSVM

/*
Support Vector Machines for Non-Linearly Separable iris data set of:
https://en.wikipedia.org/wiki/Iris_flower_data_set
modified the code in:
http://docs.opencv.org/master/d0/dcc/tutorial_non_linear_svms.html#gsc.tab=0
*/

#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <algorithm>    // std::random_shuffle
#include <time.h>	//required for RNG initializer

#define NUM_EXAMPLES 150
#define NUM_TRIALS 10
#define TRAINING_RATIO 0.9
#define RNG_SEED 2
#define numVariables 4
#define thickness -1
#define lineType 8
		 
using namespace std;
using namespace cv;
using namespace cv::ml;

//http://stackoverflow.com/questions/9435385/split-a-string-using-c11
//split the line
vector<string> split(const string& str, int delimiter(int) = ::isspace)
{
	vector<string> result;
	auto e=str.end();
	auto i=str.begin();
	while(i!=e)
	{
		i=find_if_not(i,e, delimiter);
		if(i==e) 
			break;
		auto j=find_if(i,e, delimiter);
		result.push_back(string(i,j));
		i=j;
	}
	return result;
}

//read the txt data file and write into cv::Mat variables
void readFile(string fileName, Mat& allData, Mat& allLabels)
{
	string line;
	ifstream myfile (fileName);
	int lineCount = 0;
	if (myfile.is_open())
	{
		while (getline (myfile,line) && lineCount < NUM_EXAMPLES)
		{
			vector<string> splittedData = split(line);
			for(int i=0; i<numVariables; i++)
			{
				allData.at<float>(lineCount,i) = std::stod(splittedData[i]);
			}

			string labelName = splittedData[numVariables];
			if(labelName == "I.setosa")
			{
				allLabels.at<int>(lineCount,0) = 0;
			}else if(labelName == "I.versicolor")
			{
				allLabels.at<int>(lineCount,0) = 1;
			}else if(labelName == "I.virginica")
			{
				allLabels.at<int>(lineCount,0) = 2;
			}

			lineCount++;
		}
		myfile.close();
	}

	else cout << "Unable to open file"; 
}

//modified : http://stackoverflow.com/questions/17348854/random-order-shuffle-cvmat-in-opencv
//shuffling the data and label Mat variable
void shuffleRows(const Mat& allData, const Mat& allLabels, Mat& shuffledAllData, Mat& shuffledAllLabels, int trial)
{
	std::vector <int> seeds;
	for (int cont = 0; cont < allData.rows; cont++)
	{
		seeds.push_back(cont);
	}

	std::random_shuffle(seeds.begin(), seeds.end());//http://www.cplusplus.com/reference/algorithm/random_shuffle/

	for (int cont = 0; cont < allData.rows; cont++)
	{
		allData.row(seeds[cont]).copyTo(shuffledAllData.row(cont));
		allLabels.row(seeds[cont]).copyTo(shuffledAllLabels.row(cont));
	}
}

//Error calculation after training
float errorCheck(Ptr<SVM> svm, Mat& Data, Mat& Labels)
{
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

		(response==Labels.at<int>(j,0)) ? correctCount++ : wrongCount++;
	}	
	
	return (float)wrongCount/(float)(correctCount+wrongCount);
}

int main(void)
{
	//---------------------  Reading the data and labels from iris.txt ---------------------------------------
	int numTraining = (int) (TRAINING_RATIO * NUM_EXAMPLES); //how many examples will be used for training the SVM?

	Mat allData(NUM_EXAMPLES, numVariables, CV_32FC1);
	Mat shuffledAllData(NUM_EXAMPLES, numVariables, CV_32FC1);
	Mat allLabels(NUM_EXAMPLES, 1, CV_32SC1);
	Mat shuffledAllLabels(NUM_EXAMPLES, 1, CV_32SC1);
	Mat trainData(numTraining, numVariables, CV_32FC1);
	Mat trainLabels(numTraining, 1, CV_32FC1);
	Mat testData(NUM_EXAMPLES-numTraining, numVariables, CV_32SC1);
	Mat testLabels(NUM_EXAMPLES-numTraining, 1, CV_32SC1);

	//reading the iris data set, and copying to cv::Mat variables
	readFile("iris.txt", allData, allLabels);

	float Ein_mean = 0.;
	float Eout_mean = 0.;
	float Ein;
	float Eout;
	//starting the trials	
	for (int trial = 0; trial < NUM_TRIALS ; trial++)
	{
		//shuffling cv::Mat's
		srand(time(0)+trial*1000);
		shuffleRows(allData, allLabels, shuffledAllData, shuffledAllLabels, trial);

		//extracting the training and test data from shuffled cv::Mat's
		trainData = shuffledAllData(cv::Range(0, numTraining), cv::Range(0, numVariables));
		trainLabels = shuffledAllLabels(cv::Range(0, numTraining), cv::Range(0, 1));
		testData = shuffledAllData(cv::Range(numTraining, NUM_EXAMPLES), cv::Range(0, numVariables));
		testLabels = shuffledAllLabels(cv::Range(numTraining, NUM_EXAMPLES), cv::Range(0, 1));

		//------------------------ Set up and Train the support vector machines parameters --------------------
		cout << "Starting training process of #" << trial << "\n";
		Ptr<SVM> svm = SVM::create();
		svm->setType(SVM::C_SVC);
		svm->setC(0.1);
		svm->setKernel(SVM::LINEAR);
		svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, (int)1e7, 1e-6));
		svm->train(trainData, ROW_SAMPLE, trainLabels);
		cout << "Finished training process #" << trial << "\n";
		Ein = errorCheck(svm, trainData, trainLabels);
		cout << "Ein : " << Ein << "\n";
		Eout = errorCheck(svm, testData, testLabels);
		cout << "Eout : " << Eout << "\n";
		cout << "--------------------------------\n\n";
		Ein_mean += Ein/NUM_TRIALS;
		Eout_mean += Eout/NUM_TRIALS;
	}
	
	cout << "Ein_mean : " << Ein_mean << " after " << NUM_TRIALS << " trials.\n";
	cout << "Eout_mean : " << Eout_mean << " after " << NUM_TRIALS << " trials.\n\n";
}

