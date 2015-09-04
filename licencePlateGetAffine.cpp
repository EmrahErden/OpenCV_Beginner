//g++ licencePlateGetAffine.cpp `pkg-config --libs opencv` -o licencePlateGetAffine
//modified the code on: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html

/*
Loading an image with transformed licence plate. User selects corner point #1,2,3 of a licence plate and presses enter.
Then selected region is affine transformed, and cropped. 
*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <string>

std::string getImageType(int number)
{
    // find type
    int imgTypeInt = number%8;
    std::string imgTypeString;

    switch (imgTypeInt)
    {
        case 0:
            imgTypeString = "8U";
            break;
        case 1:
            imgTypeString = "8S";
            break;
        case 2:
            imgTypeString = "16U";
            break;
        case 3:
            imgTypeString = "16S";
            break;
        case 4:
            imgTypeString = "32S";
            break;
        case 5:
            imgTypeString = "32F";
            break;
        case 6:
            imgTypeString = "64F";
            break;
        default:
            break;
    }

    // find channel
    int channel = (number/8) + 1;

    std::stringstream type;
    type<<"CV_"<<imgTypeString<<"C"<<channel;

    return type.str();
}

using namespace std;
using namespace cv;

vector<Point2f> inputPoints(3);
vector<Point2f> outputPoints(3);
int counter = 0;
 
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        Point2f pt(x,y);
        inputPoints[counter] = pt;
        counter++;
        cout << "Point # " << counter << " location: " << x << ", " << y << "\n";
    }
    /*
    else if  ( event == EVENT_RBUTTONDOWN )
    {
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_MBUTTONDOWN )
    {
        cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if ( event == EVENT_MOUSEMOVE )
    {
        cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
    } 
    */   
}
 
int main()
{
    // Read image from file 
    Mat img = imread("licencePlate.jpg");


    //if fail to read the image
    if ( img.empty() ) 
    { 
        cout << "Error loading the image" << endl;
        return -1; 
    }
   
    cout << getImageType(img.type()) << "\n";

    //Create a window
    namedWindow("ImageDisplay", 1);

    //set the callback function for any mouse event
    setMouseCallback("ImageDisplay", CallBackFunc, NULL);
 
    //show the image
    imshow("ImageDisplay", img);
 
    // Wait until user is ready after 3 input points
    waitKey(0);
    destroyWindow("ImageDisplay");

    //calculating new locations
    outputPoints[0].x = inputPoints[0].x; 
    outputPoints[0].y = inputPoints[0].y; 
    int Iwidth = inputPoints[1].x - inputPoints[0].x;
    outputPoints[1].x = outputPoints[0].x + Iwidth;
    outputPoints[1].y = outputPoints[0].y;
    int Iheight = inputPoints[2].y - inputPoints[1].y;
    outputPoints[2].x = outputPoints[1].x;
    outputPoints[2].y = outputPoints[1].y + Iheight;
    //outputPoints[3].x = outputPoints[0].x;
    //outputPoints[3].y = outputPoints[0].y + Iheight;	

    cout << "height of selection: " << Iheight << "\n";
    cout << "width of selection: " << Iwidth << "\n";

    Mat affine_matrix, cropped, result;
    affine_matrix = getAffineTransform (inputPoints, outputPoints);

    //warpPerspective( img, result, affine_matrix, result.size() );
    warpAffine(img, result, affine_matrix, img.size());

    Rect roi(outputPoints[0].x, outputPoints[0].y, Iwidth, Iheight);
    Mat licence = result(roi);

    namedWindow("affine", WINDOW_AUTOSIZE);
    imshow("affine", licence);
    waitKey(0);

    return 0;
 
}
