#pragma once
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world454d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world454.lib")
#endif

class RealImage
{
private:
    Mat img;
    int id;
    int slaveId;
public:
    RealImage()
    {
        this->slaveId = -1;
    }

    RealImage(int id, int slaveId, Mat img)
    {
        this->id = id;
        this->slaveId = slaveId;
        this->img = img;
        //for (int y = 0; y < img.rows; ++y) {
        //    for (int x = 0; x < img.cols; ++x) {
        //        Vec3b pixel = img.at<Vec3b>(y, x);
        //        Vec3b pixel1 = this->img.at<Vec3d>(y, x);
        //        pixel1[0] = pixel[0];
        //        pixel1[1] = pixel[1];
        //        pixel1[2] = pixel[2];
        //    }
        //}
    }
    int getId()
    {
        return this->id;
    }
    int getSlaveId()
    {
        return this->slaveId;
    }
    Mat getImg()
    {
        return this->img;
    }
};


