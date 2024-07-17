#include <opencv2/opencv.hpp>
#include "matchFeatures .h"
using namespace cv;

void detectAndComputeFeatures(const Mat& img, std::vector<KeyPoint>& keypoints, Mat& descriptors)
{
    // יצירת אובייקט ORB
    Ptr<ORB> orb = ORB::create();
    // זיהוי נקודות עניין ומחשוב תיאורים
    orb->detectAndCompute(img, noArray(), keypoints, descriptors);
}

int main()
{
    // טעינת התמונה
    Mat img1 = imread("image_1.jpg", IMREAD_GRAYSCALE);

    if (img1.empty())
    {
        std::cout << "Could not open or find image1!" << std::endl;
        return -1;
    }
    resize(img1, img1, img1.size() / 5);

    // טעינת תמונה שנייה
    Mat img2 = imread("image_2.jpg", IMREAD_GRAYSCALE);

    if (img2.empty())
    {
        std::cout << "Could not open or find image2!" << std::endl;
        return -1;
    }
    resize(img2, img2, img2.size() / 5);

    // וקטור לאחסון נקודות עניין עבור כל תמונה
    std::vector<KeyPoint> keypoints1, keypoints2;

    // מטריצה לאחסון תיאורים עבור כל תמונה
    Mat descriptors1, descriptors2;

    // זיהוי נקודות עניין ומחשוב תיאורים עבור תמונה ראשונה
    detectAndComputeFeatures(img1, keypoints1, descriptors1);

    // זיהוי נקודות עניין ומחשוב תיאורים עבור תמונה שנייה
    detectAndComputeFeatures(img2, keypoints2, descriptors2);
   
   
    // התאמת נקודות עניין באמצעות FLANN
    FlannBasedMatcher matcher(new flann::LshIndexParams(20, 10, 2));
    std::vector<std::vector<DMatch>> knn_matches;
    
    matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);

    MatchFeaturers matcher1;
    //std::vector<std::vector<PointMatch>> knn_matche;
    //matcher1.knnMatch(descriptors1, descriptors2, knn_matche, 2);
   // matcher1.matchFilter(MatchFeaturers::knn_matche);
    //// סינון התאמות טובות (KNN)
    std::vector<DMatch> good_matches;

    for (int i = 0; i < knn_matches.size(); ++i)
    {
        if (knn_matches[i][0].distance < 0.7 * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    // ציור התאמות טובות
    Mat result;
    drawMatches(img1, keypoints1, img2, keypoints2, good_matches, result);
    imshow("Matches", result);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
