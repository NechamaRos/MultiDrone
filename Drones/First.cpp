

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "opencv2/stitching.hpp"
#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world4100d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world4100d.lib")
#endif
using namespace cv;
using namespace std;


int main1()
{
    Stitcher::Mode mode = Stitcher::SCANS;

    // Array for pictures
    vector<Mat> imgs;
    // Get all the images that need to be 
 // stitched as arguments from command line 

        // Read the ith argument or image 
        // and push into the image array
    for (int i = 1; i <= 4; i ++ )
    {
        stringstream ss;
        ss<<"image_"<<i<<".jpg";
        Mat img = imread("map\\"+ss.str());
        double scaleFactor = 0.3;  // Adjust based on desired reduction
        resize(img, img, Size(), scaleFactor, scaleFactor, INTER_AREA);
//imread(ss.str(), IMREAD_COLOR, img); // Read in color mode
        //if (scaleFactor < 1.0) {
        //    resize(img, img, Size(), scaleFactor, scaleFactor, INTER_AREA);
        //}
        imgs.push_back(img);
    }
    cout << imgs.size();
     


      

    // Define object to store the stitched image
    Mat pano;

    // Create a Stitcher class object with mode panoroma
    Ptr<Stitcher> stitcher = Stitcher::create(mode);

    // Command to stitch all the images present in the image array
    Stitcher::Status status = stitcher->stitch(imgs, pano);
    if (status != Stitcher::OK)
    {
        // Check if images could not be stitched
        // status is OK if images are stitched successfully
        cout << "Can't stitch images\n";
        return -1;
    }

    // Store a new image stitched from the given 
    //set of images as "result.jpg"
    imwrite("result.jpg", pano);

    // Show the result
    imshow("Result", pano);


   
    waitKey(0);
    return 0;
}