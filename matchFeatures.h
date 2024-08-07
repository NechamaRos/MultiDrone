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

class MatchFeatures {
public:

    //C'tor
    MatchFeatures();

    class DMatch {

    public:
        int queryIdx;
        int trainIdx;
        float distance;

        //deafult C'tor
        DMatch();

        //C'tor
        DMatch(int qIdx, int tIdx, float dist);

        //return true if this.distance > other.distance
        bool operator>(const DMatch& other);

        //get queryIdx
        int first();

        //get trainIdx
        int second();
    };

    // Function to compute Euclidean distance between two descriptors
    double euclideanDistance(const std::vector<float>& desc1, const std::vector<float>& desc2);

    // Function to perform KNN matching
    std::vector<std::vector<MatchFeatures::DMatch>> knnMatch(const std::vector<std::vector<float>>& descriptors1, const std::vector<std::vector<float>>& descriptors2, int k);

    // Function to match features between two sets of descriptors using KNN
    vector<DMatch> matchFeatures(const std::vector<std::vector<float>>& descriptors1, const std::vector<std::vector<float>>& descriptors2);

    void drawMatches(const cv::Mat& img1, const std::vector<cv::KeyPoint>& keypoints1,
        const cv::Mat& img2, const std::vector<cv::KeyPoint>& keypoints2,
        const std::vector<DMatch>& matches, cv::Mat& outImg);

    map<int, int> countKeypointOccurrences(const vector<MatchFeatures::DMatch>& matches);

};