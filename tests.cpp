#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#ifdef _DEBUG 
//the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world4100d.lib")
#else
the release version that corresponds to the opencv_world420.dll :
#pragma comment(lib, "opencv_world4100.lib")
#endif

#include "doctest.h"
#include "matchFeatures.h"
#include <algorithm>
#include <iostream>

static cv::Mat img1, img2;
static cv::Mat descriptors1, descriptors2;

std::vector<MatchFeatures::DMatch1> convertToDMatch1(const std::vector<cv::DMatch>& matches) {
    std::vector<MatchFeatures::DMatch1> matches1;
    for (const auto& match : matches) {
        matches1.push_back(MatchFeatures::DMatch1{ match.queryIdx, match.trainIdx, match.distance });
    }
    return matches1;
}


void bruteForceKnnMatch(const cv::Mat& descriptors1, const cv::Mat& descriptors2, vector<vector<cv::DMatch>>& knn_matches)
{
    // Create BFMatcher object with NORM_L2
    cv::BFMatcher bfmatcher(cv::NORM_L2);
    // Perform KNN match
    bfmatcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);
}

std::vector<std::vector<float>> matToVector(const cv::Mat& mat) {
    std::vector<std::vector<float>> vec(mat.rows, std::vector<float>(mat.cols));
    for (int i = 0; i < mat.rows; ++i) {
        mat.row(i).copyTo(cv::Mat(vec[i]).reshape(1, 1));
    }
    return vec;
}

bool areVectorsEqual(const std::vector<MatchFeatures::DMatch1>& vec1, const std::vector<MatchFeatures::DMatch1>& vec2) {
    if (vec1.size() != vec2.size()) return false;
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i].queryIdx != vec2[i].queryIdx ||
            vec1[i].trainIdx != vec2[i].trainIdx ||
            std::fabs(vec1[i].distance - vec2[i].distance) > 1e-6) {
            return false;
        }
    }
    return true;
}

TEST_CASE("Setup OpenCV logging level") {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
}

TEST_CASE("check the image size and not empty") {
   
    //img1 = cv::imread("D:\\Users\\The user\\Downloads\\image_1.jpg", cv::IMREAD_COLOR);
    //img2 = cv::imread("D:\\Users\\The user\\Downloads\\image_2.jpg", cv::IMREAD_COLOR);

    img1 = cv::imread("D:\\Users\\The user\\Downloads\\תמונות פנורמה\\medium11.jpg", cv::IMREAD_COLOR);
    img2 = cv::imread("D:\\Users\\The user\\Downloads\\תמונות פנורמה\\medium12.jpg", cv::IMREAD_COLOR);

    CHECK(img1.size() == img2.size());
    CHECK(img1.empty() == false);
    CHECK(img2.empty() == false);
}

TEST_CASE("check that the descriptors are not empty") {

    cv::Mat gray1, gray2;
    cvtColor(img1, gray1, cv::COLOR_BGR2GRAY);
    cvtColor(img2, gray2, cv::COLOR_BGR2GRAY);

    // Detect and compute descriptors
    cv::Ptr<cv::SIFT> detector = cv::SIFT::create();
    vector<cv::KeyPoint> keypoints1, keypoints2;

    detector->detectAndCompute(gray1, cv::noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(gray2, cv::noArray(), keypoints2, descriptors2);

    CHECK_FALSE(descriptors1.empty());
    CHECK_FALSE(descriptors2.empty());

}

TEST_CASE("found same matches") {

    vector<vector<cv::DMatch>> knn_matches;
    bruteForceKnnMatch(descriptors1, descriptors2, knn_matches);

    // Filter matches using Lowe's ratio test
    const float ratio_thresh = 0.7f;
    vector<cv::DMatch> matchesCV;
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
            matchesCV.push_back(knn_matches[i][0]);
        }
    }

    MatchFeatures matcher;

    std::vector<std::vector<float>> descVec1 = matToVector(descriptors1);
    std::vector<std::vector<float>> descVec2 = matToVector(descriptors2);

    vector<MatchFeatures::DMatch1> matches = matcher.matchFeatures(descVec1, descVec2);

    std::vector<MatchFeatures::DMatch1> matchesCVConvert = convertToDMatch1(matchesCV);

    std::cout << "Testing found same matches: " << std::endl;
    std::cout << "matchesCV size: " << matchesCVConvert.size() << std::endl;
    std::cout << "matches size: " << matches.size() << std::endl;

    CHECK(matches.size() == matchesCVConvert.size());

    for (size_t i = 0; i < matchesCVConvert.size(); i++)
    {
        std::cout << "Match " << i << ": " << std::endl;
        //std::cout << "QueryIdx: expected = " << matchesCVConvert.at(i).queryIdx << ", actual = " << matches.at(i).queryIdx << std::endl;
        //std::cout << "TrainIdx: expected = " << matchesCVConvert.at(i).trainIdx << ", actual = " << matches.at(i).trainIdx << std::endl;
        //std::cout << "Distance: expected = " << matchesCVConvert.at(i).distance << ", actual = " << matches.at(i).distance << std::endl;

        CHECK(matchesCVConvert.at(i).queryIdx == matches.at(i).queryIdx);
        CHECK(matchesCVConvert.at(i).trainIdx == matches.at(i).trainIdx);
        CHECK(matchesCVConvert.at(i).distance == matches.at(i).distance);
    }

}

TEST_CASE("testing the matchFeatures function for equality of matches") {
    MatchFeatures matcher;

    std::vector<std::vector<float>> descriptors1 = {
        {0.1f, 0.2f, 0.3f},
        {0.4f, 0.5f, 0.6f},
        {0.7f, 0.8f, 0.9f},
        {1.0f, 1.1f, 1.2f}
    };

    std::vector<std::vector<float>> descriptors2 = {
        {0.1f, 0.2f, 0.3f},
        {0.7f, 0.8f, 0.9f},
        {0.4f, 0.5f, 0.6f},
        {1.0f, 1.1f, 1.2f}
    };

    std::vector<MatchFeatures::DMatch1> expectedMatches = {
        MatchFeatures::DMatch1(0, 0, 0.0f),
        MatchFeatures::DMatch1(1, 2, 0.0f),
        MatchFeatures::DMatch1(2, 1, 0.0f),
        MatchFeatures::DMatch1(3, 3, 0.0f)
    };

    std::vector<MatchFeatures::DMatch1> actualMatches = matcher.matchFeatures(descriptors1, descriptors2);

    std::cout << "Testing matchFeatures for equality of matches: " << actualMatches.size() << " matches" << std::endl;
    CHECK(areVectorsEqual(actualMatches, expectedMatches));
}