#pragma once

#include <vector>
#include <limits>
#include <cmath>
#include <cfloat>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world4100d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world4100.lib")
#endif

using namespace std;
//using namespace cv;

class MatchFeatures {
public:

    //בנאי ריק
    MatchFeatures();

    class DMatch1 {
    public:
        int queryIdx;
        int trainIdx;
        float distance;

        DMatch1() : queryIdx(-1), trainIdx(-1), distance(FLT_MAX) {}
        DMatch1(int qIdx, int tIdx, float dist) : queryIdx(qIdx), trainIdx(tIdx), distance(dist) {}

        bool operator>(const DMatch1& other) const {
            return this->distance > other.distance;
        }
        // Add member functions to mimic `first` and `second` access
        int getFirst() const { return queryIdx; }
        int getSecond() const { return trainIdx; }

        // Optionally, you can add operators for convenience
        int first() const { return queryIdx; }
        int second() const { return trainIdx; }
    };

    // פונקציה לחשב את המרחק האוקלידי בין שני תיאורים
    double euclideanDistance(const std::vector<float>& desc1, const std::vector<float>& desc2);

    std::vector<std::vector<MatchFeatures::DMatch1>> knnMatch(const std::vector<std::vector<float>>& descriptors1, const std::vector<std::vector<float>>& descriptors2, int k);

    // פונקציה להתאמת תכונות בין שתי תמונות
    vector<DMatch1> matchFeatures(const std::vector<std::vector<float>>& descriptors1, const std::vector<std::vector<float>>& descriptors2);
    //void knnMatchImpl1(const cv::Mat& queryDescriptors, const cv::Mat& trainDescriptors, vector<vector<MatchFeatures::DMatch1>>& matches, int knn);
    //float normL2Sqr1(const cv::Mat& vec1, const cv::Mat& vec2);


    // ציור ההתאמות בין שתי תמונות
    void drawMatches(const cv::Mat& img1, const std::vector<cv::KeyPoint>& keypoints1,
        const cv::Mat& img2, const std::vector<cv::KeyPoint>& keypoints2,
        const std::vector<DMatch1>& matches, cv::Mat& outImg);

    //ספירת כמות פעמים של התאמות לנקודה
    map<int, int> countKeypointOccurrences(const vector<MatchFeatures::DMatch1>& matches);

};