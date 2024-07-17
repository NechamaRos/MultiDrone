//#include "matchFeatures .h"
//#include <opencv2/opencv.hpp>
//using namespace cv;
//void detectAndComputeFeatures(const Mat& img, std::vector<KeyPoint>& keypoints, Mat& descriptors)
//{
//    // יצירת אובייקט ORB
//    Ptr<ORB> orb = ORB::create();
//    // זיהוי נקודות עניין ומחשוב תיאורים
//    orb->detectAndCompute(img, noArray(), keypoints, descriptors);
//}
//int main_()
//{
//    Mat img1 = imread("image_1.jpg", IMREAD_GRAYSCALE);
//    if (img1.empty())
//    {
//        std::cout << "Could not open or find image1!" << std::endl;
//        return -1;
//    }
//    resize(img1, img1, img1.size() / 5);
//    Mat img2 = imread("image_2.jpg", IMREAD_GRAYSCALE);
//    if (img2.empty())
//    {
//        std::cout << "Could not open or find image2!" << std::endl;
//        return -1;
//    }
//    resize(img2, img2, img2.size() / 5);
//    std::vector<KeyPoint> keypoints1, keypoints2;
//    Mat descriptors1, descriptors2;
//    detectAndComputeFeatures(img1, keypoints1, descriptors1);
//    detectAndComputeFeatures(img2, keypoints2, descriptors2);
//    ////////////////////////////////////////////////////////////
//    MatchFeaturers matcher;
//    std::vector<std::vector<PointMatch>> knn_matches;
//    matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);
//}