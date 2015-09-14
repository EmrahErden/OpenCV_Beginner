//g++ -std=c++11 binaryFileDisplay.cpp `pkg-config --libs opencv` -o binaryFileDisplay

/*
1st:
Reading the out MNIST DATABASE data set from binary files:
http://yann.lecun.com/exdb/mnist/

http://www.cplusplus.com/doc/tutorial/files/

For binary files, reading and writing data with the extraction and insertion operators (<< and >>) and functions like getline is not efficient, since we do not need to format any data and data is likely not formatted in lines.

File streams include two member functions specifically designed to read and write binary data sequentially: write and read. The first one (write) is a member function of ostream (inherited by ofstream). And read is a member function of istream (inherited by ifstream). Objects of class fstream have both. Their prototypes are:

write ( memory_block, size );
read ( memory_block, size );

Where memory_block is of type char* (pointer to char), and represents the address of an array of bytes where the read data elements are stored or from where the data elements to be written are taken. The size parameter is an integer value that specifies the number of characters to be read or written from/to the memory block.

2nd:
After saving the data into cv::Mat files, samples are visualy tested via testDisplay function
*/

#include <iostream>
#include <fstream>
#include <opencv2/highgui.hpp>

#define IMAGE_SIZE 28
#define MINST_OFFSET_DF 16 //offset in data file in terms of bytes
#define MINST_OFFSET_LF 8 //offset in labels file in terms of bytes
#define NUM_TRAINDATA 60000 //number of examples in the training data set

using namespace std;
using namespace cv;

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
	cout << "Displayed image has value of : " << (int)Labels.at<uchar>(displayLoc,0) <<"\n"; 
	waitKey(0);
}


int main(void)
{
	Mat trainData(NUM_TRAINDATA, IMAGE_SIZE*IMAGE_SIZE, CV_8UC1);
	Mat trainLabels(NUM_TRAINDATA, 1, CV_8UC1);
	extractBinaryDataFile(trainData, "train-images-idx3-ubyte", NUM_TRAINDATA);
	extractBinaryLabelsFile(trainLabels, "train-labels.idx1-ubyte", NUM_TRAINDATA);
	testDisplay(trainData, trainLabels, 59999);
}

