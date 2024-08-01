#include "matchFeatures.h"
#include <omp.h> // Include OpenMP header
// Constructor
MatchFeatures::MatchFeatures() {}

// Function to compute Euclidean distance between two descriptors
double MatchFeatures::euclideanDistance(const std::vector<float>& desc1, const std::vector<float>& desc2)
{
    double sum = 0.0;
    for (size_t i = 0; i < desc1.size(); ++i) {
        double diff = desc1[i] - desc2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// Function to perform KNN matching
std::vector<std::vector<MatchFeatures::DMatch1>> MatchFeatures::knnMatch(const std::vector<std::vector<float>>& descriptors1, const std::vector<std::vector<float>>& descriptors2, int k)
{
    std::vector<std::vector<DMatch1>> knn_matches;
#pragma omp parallel for
    for (int i = 0; i < descriptors1.size(); ++i)
    {
        std::vector<std::pair<int, double>> dists;

        for (int j = 0; j < descriptors2.size(); ++j)
        {
            double dist = MatchFeatures::euclideanDistance(descriptors1[i], descriptors2[j]);
            dists.push_back(std::make_pair(j, dist));
        }

        std::sort(dists.begin(), dists.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
            return a.second < b.second;
            });


        std::vector<MatchFeatures::DMatch1> knn;
        for (int n = 0; n < k && n < dists.size(); ++n)
        {
            knn.push_back(MatchFeatures::DMatch1(i, dists[n].first, static_cast<float>(dists[n].second)));
        }
        knn_matches.push_back(knn);
    }

    return knn_matches;
}

// Function to match features between two sets of descriptors using KNN
std::vector<MatchFeatures::DMatch1> MatchFeatures::matchFeatures(const std::vector<std::vector<float>>& descriptors1, const std::vector<std::vector<float>>& descriptors2)
{
    std::vector<DMatch1> matches;
    const float ratio_thresh = 0.7f;
    int k = 2; // Number of neighbors to consider

    std::vector<std::vector<DMatch1>> knn_matches = MatchFeatures::knnMatch(descriptors1, descriptors2, k);

    for (const auto& knn : knn_matches)
    {
        if (knn.size() == k && knn[0].distance < ratio_thresh * knn[1].distance)
        {
            matches.push_back(knn[0]);
        }
    }

    std::cout << "Number of good matches: " << matches.size() << std::endl;
    return matches;
}

// Function to draw matches between two images
void MatchFeatures::drawMatches(const cv::Mat& img1, const std::vector<cv::KeyPoint>& keypoints1,
    const cv::Mat& img2, const std::vector<cv::KeyPoint>& keypoints2,
    const std::vector<DMatch1>& matches, cv::Mat& outImg)
{
    if (matches.empty()) {
        outImg = cv::Mat::zeros(img1.rows, img1.cols + img2.cols, CV_8UC3);
        return;
    }

    std::vector<cv::Scalar> colors = {
        cv::Scalar(255, 0, 0),
        cv::Scalar(0, 255, 0),
        cv::Scalar(0, 0, 255),
        cv::Scalar(255, 255, 0),
        cv::Scalar(255, 0, 255),
        cv::Scalar(0, 255, 255),
        cv::Scalar(128, 0, 128),
        cv::Scalar(255, 165, 0)
    };

    cv::Mat combined_img;
    cv::hconcat(img1, img2, combined_img);
    outImg = combined_img.clone();

    for (size_t i = 0; i < matches.size(); ++i) {
        cv::Point2f pt1 = keypoints1[matches[i].queryIdx].pt;
        cv::Point2f pt2 = keypoints2[matches[i].trainIdx].pt + cv::Point2f(static_cast<float>(img1.cols), 0);

        cv::Scalar color = colors[i % colors.size()];
        cv::line(outImg, pt1, pt2, color, 1);
        cv::circle(outImg, pt1, 3, color, 1);
        cv::circle(outImg, pt2, 3, color, 1);
    }
}
map<int, int> MatchFeatures::countKeypointOccurrences(const vector<MatchFeatures::DMatch1>& matches)
{
    map<int, int> keypointCount;

    // Iterate over matches and count occurrences of each keypoint
    for (const auto& match : matches) {
        keypointCount[match.queryIdx]++;
        keypointCount[match.trainIdx]++;
    }

    return keypointCount;
}

