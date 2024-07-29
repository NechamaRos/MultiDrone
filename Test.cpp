#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#ifdef _DEBUG
 //the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world4100d.lib")
#else
 //the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world4100.lib")
#endif

#include "doctest.h"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
//#include "1.cpp"
#include "FlannMatcher.cpp"
#include <iostream>

using namespace cv::flann;
using namespace cv;
using namespace std;


void detectAndComputeFeatures(const Mat& img, std::vector<KeyPoint>& keypoints, Mat& descriptors) {
    // Create ORB object
    Ptr<ORB> orb = ORB::create();
    // Detect keypoints and compute descriptors
    orb->detectAndCompute(img, noArray(), keypoints, descriptors);
}

std::vector<std::vector<int>> matToVector(const cv::Mat& mat) {
    std::vector<std::vector<int>> vec(mat.rows, std::vector<int>(mat.cols));
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            vec[i][j] = static_cast<int>(*(mat.ptr(i, j)));
        }
    }
    return vec;
}


//
//
////void resize(const Mat& src, Mat& dst, Size dsize, int interpolation) {
////    // בדיקת תנאים מקדימים (למשל, שהתמונה המקורית אינה ריקה)
////    CV_Assert(!src.empty());
////
////    // יצירת התמונה החדשה בגודל הרצוי
////    dst.create(dsize, src.type());
////
////    // חישוב גורמי קנה המידה
////    float fx = (float)dsize.width / src.cols;
////    float fy = (float)dsize.height / src.rows;
////
////    // איטרציה על כל פיקסל בתמונה החדשה
////    for (int y = 0; y < dst.rows; ++y) {
////        for (int x = 0; x < dst.cols; ++x) {
////            // חישוב הקואורדינטות בתמונה המקורית
////            float srcX = x / fx;
////            float srcY = y / fy;
////
////            // בחירת שיטת האינטרפולציה המתאימה
////            switch (interpolation) {
////            case INTER_NEAREST:
////                // אינטרפולציה הקרובה ביותר
////                dst.at<uchar>(y, x) = src.at<uchar>(cvRound(srcY), cvRound(srcX));
////                break;
////                // ... מקרים נוספים לשיטות אינטרפולציה אחרות
////            }
////        }
////    }
////}
//
////TEST_CASE("check the image size") {
////    Mat img1 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\1.jpg", IMREAD_GRAYSCALE);
////    Mat img2 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\2.jpg", IMREAD_GRAYSCALE);
////    Mat img3 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\1.jpg", IMREAD_GRAYSCALE);
////
////   
////    resize(img1, img1, img1.size() / 3);
////    resize(img2, img2, img2.size() / 3);
////
////    CHECK(img1.size() == img2.size());
////    CHECK_FALSE(img1.size() == img3.size());
////}
//TEST_CASE("input is not empty") {
//   Mat img1 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\1.jpg", IMREAD_GRAYSCALE);
//   Mat img2 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\2.jpg", IMREAD_GRAYSCALE);
//   std::vector<KeyPoint> keypoints1, keypoints2;
//   Mat descriptors1, descriptors2;
//   detectAndComputeFeatures(img1, keypoints1, descriptors1);
//   detectAndComputeFeatures(img2, keypoints2, descriptors2);
//   CHECK_FALSE(keypoints1.empty());
//
//   CHECK(keypoints2.empty() == false);
//
//}
//TEST_CASE("check Input for lsh table") {
//    int table_number = 10;
//    int key_size = 5;
//    int multi_probe_level = 2;
//    cout << "check Input for lsh table\n";
//    CHECK_NE(table_number, 0);
//    cout << "table number could not be 0\n";
//    FlannBasedMatcher matcher(new flann::LshIndexParams(table_number, key_size, multi_probe_level)); // Example FLANN parameters
//
//    //REQUIRE_NE(table_number, 0);
//}
//
//
//TEST_CASE("matches same picture") {
//    Mat img1 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\1.jpg", IMREAD_GRAYSCALE);
//
//    std::vector<KeyPoint> keypoints1;
//    Mat descriptors1;
//    detectAndComputeFeatures(img1, keypoints1, descriptors1);
//    FlannBasedMatcher matcher(new flann::LshIndexParams(20, 10, 2)); // Example FLANN parameters
//    std::vector<std::vector<DMatch>> knn_matches;
//    matcher.knnMatch(descriptors1, descriptors1, knn_matches, 2);
//    // סינון התאמות טובות (KNN)
//    std::vector<DMatch> good_matches;
//    for (int i = 0; i < knn_matches.size(); ++i) {
//        if (knn_matches[i][0].distance < 0.7 * knn_matches[i][1].distance) {
//            good_matches.push_back(knn_matches[i][0]);
//        }
//    }
//    for (size_t i = 0; i < good_matches.size(); i++) {
//        DMatch match = good_matches[i];
//        CHECK_EQ(match.queryIdx, match.trainIdx);
//        //CHECK_NE(match.queryIdx, match.trainIdx);
//    }
//}
//
//TEST_CASE("matches opencv few images") {
//   
//    for (int i = 1; i <10; ++i)
//    {
//        stringstream s;
//        s << i << ".jpg";
//        Mat img = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\" + s.str());
//
//        if (img.empty())
//        {
//            
//            cout << "Can't read image '" << i << "\n";
//           
//        }
//
//    std::vector<KeyPoint> keypoints1;
//    Mat descriptors1;
//    detectAndComputeFeatures(img, keypoints1, descriptors1);
//    FlannBasedMatcher matcher(new flann::LshIndexParams(20, 10, 2)); // Example FLANN parameters
//    std::vector<std::vector<DMatch>> knn_matches;
//    matcher.knnMatch(descriptors1, descriptors1, knn_matches, 2);
//    // סינון התאמות טובות (KNN)
//    std::vector<DMatch> good_matches;
//    for (int i = 0; i < knn_matches.size(); ++i) {
//        if (knn_matches[i][0].distance < 0.7 * knn_matches[i][1].distance) {
//            good_matches.push_back(knn_matches[i][0]);
//        }
//    }
//    cout << i+ "=>begin\n";
//    for (size_t i = 0; i < good_matches.size(); i++) {
//        DMatch match = good_matches[i];
//        CHECK_EQ(match.queryIdx, match.trainIdx);
//        //CHECK_NE(match.queryIdx, match.trainIdx);
//
//    }
//    cout <<i+ "= >end\n";
//    }
//}
//
//
//
//TEST_CASE("MATCHES with our code") {
//    // Load the first image
//    Mat img1 = imread("image_1.jpg", IMREAD_GRAYSCALE);
//    if (img1.empty()) {
//        std::cout << "Could not open or find image1!\n" << std::endl;
//       
//    }
//    resize(img1, img1, img1.size() / 5);
//
//    // Load the second image
//    Mat img2 = imread("image_1.jpg", IMREAD_GRAYSCALE);
//    if (img2.empty()) {
//        std::cout << "Could not open or find image2!\n" << std::endl;
//        
//    }
//    resize(img2, img2, img2.size() / 5);
//
//    // Vectors to store keypoints for each image
//    std::vector<KeyPoint> keypoints1, keypoints2;
//
//    // Matrices to store descriptors for each image
//    Mat descriptors1, descriptors2;
//
//    // Detect keypoints and compute descriptors for the first image
//    detectAndComputeFeatures(img1, keypoints1, descriptors1);
//
//    // Detect keypoints and compute descriptors for the second image
//    detectAndComputeFeatures(img2, keypoints2, descriptors2);
//
//    std::vector<std::vector<int>> descriptors1_ = matToVector(descriptors1);
//    std::vector<std::vector<int>> descriptors2_ = matToVector(descriptors2);
//
//    MatchFeaturers matcher1;
//    std::vector<PointMatch> knn_matches_ = matcher1.knnMatch(descriptors1_, descriptors2_, 1);
//
//    // Filter matches
//    //std::vector<PointMatch> good_matches = matcher1.matchFilter(knn_matches_);
//
//    // Drawing the good matches
//    std::vector<DMatch> good_dmatches;
//    for (const auto& match : knn_matches_) {
//        good_dmatches.push_back(DMatch(match.queryIdx, match.trainIdx, match.distance));
//    }
//    for (size_t i = 0; i < good_dmatches.size(); i++) {
//        DMatch match = good_dmatches[i];
//        CHECK_EQ(match.queryIdx, match.trainIdx);
//        //CHECK_NE(match.queryIdx, match.trainIdx);
//    }
//}
    
TEST_CASE("equals the results") {
    for (int i = 1; i < 11;i+=2)
    {
        // Load the first image
        stringstream s;
        s << i << ".jpg";
        Mat img1 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\" + s.str(),IMREAD_GRAYSCALE);

        if (img1.empty()) {
            std::cout << "Could not open or find image!\n" << std::endl;
        }
        resize(img1, img1, img1.size() / 5);

        // Load the second image
        stringstream s2;
        s2 << i+1 << ".jpg";
        Mat img2 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\" + s2.str(),IMREAD_GRAYSCALE);
        if (img2.empty()) {
            std::cout << "Could not open or find image!\n" << std::endl;
        }
        resize(img2, img2, img2.size() / 5);

 
        // Vectors to store keypoints for each image
        std::vector<KeyPoint> keypoints1, keypoints2;
        // Matrices to store descriptors for each image
        Mat descriptors1, descriptors2;
        // Detect keypoints and compute descriptors for the first image
        detectAndComputeFeatures(img1, keypoints1, descriptors1);
        // Detect keypoints and compute descriptors for the second image
        detectAndComputeFeatures(img2, keypoints2, descriptors2);
        // Convert cv::Mat to std::vector<std::vector<float>>
        std::vector<std::vector<int>> descriptors1_ = matToVector(descriptors1);
        std::vector<std::vector<int>> descriptors2_ = matToVector(descriptors2);
        //Perform matching
        FlannBasedMatcher matcher(new flann::LshIndexParams(20, 10, 2));
        std::vector<std::vector<DMatch>> knn_matches;
        matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);
        std::vector<DMatch> good;
        cout << "good without filter\n" << knn_matches.size()<<"\n";
        for (const auto& matches : knn_matches) {
            if (matches.size() > 1) {
                // בדיקת יחס המרחקים בין ההתאמה הראשונה לשנייה
                if (matches[0].distance < 0.7 * matches[1].distance) {
                    good.push_back(matches[0]);
                }
            }
        }
        Mat res;
        drawMatches(img1, keypoints1, img2, keypoints2, good, res);
        imwrite("open cv.jpg", res);
        MatchFeaturers matcher1;
        std::vector<std::vector<PointMatch>> knn_matches_ = matcher1.knnMatch(descriptors1_, descriptors2_, 2);
        std::vector<PointMatch> good_dmatches;
        cout << "goodmatches without filter\n" << knn_matches_.size()<<"\n";

        for (const auto& matches : knn_matches_) {
            // good.push_back(matches[0]);
            if (matches.size() > 1) {
                // Checking the distance ratio between the first match and the second
                if (matches[0].distance < 0.7 * matches[1].distance) {
                    good_dmatches.push_back(matches[0]);
                }
            }
        }
        std::vector<DMatch> dmatch;

        for (const auto& match : good_dmatches) {
            dmatch.push_back(DMatch(match.queryIdx, match.trainIdx, match.distance));
        }


        int count = 0;
        for (size_t j = 0; j < good_dmatches.size(); j++) {
                PointMatch query = good_dmatches[j];
                auto it = std::find_if(good.begin(), good.end(), [&query](const DMatch& m)
                    { return query.queryIdx == m.queryIdx && query.trainIdx == m.trainIdx; });
                if (it != good.end()) {
                    count++;
                }

                //CHECK_NE(match.queryIdx, match.trainIdx);
            }
        //for (size_t j = 0; j < good_dmatches.size(); j++) {
        //    DMatch query = good_dmatches[i];

        //    auto it = std::find_if(good.begin(), good.end(), [&query](const DMatch& m)
        //        { return query.queryIdx == m.queryIdx && query.trainIdx == m.trainIdx; });
        //    if (it != good.end()) {
        //        count++;
        //    }

        //    //CHECK_NE(match.queryIdx, match.trainIdx);
        //}
        cout << "good " << good.size()<<"\n";
        cout << "good_match " << good_dmatches.size() << "\n";;
        cout << "between " <<s.str() <<" and " << s2.str() <<" is " <<count << "\n";
        //REQUIRE((good_dmatches.size() * 1.14) < good.size());
        
        //CHECK(count>good.size()*0.5);
       
    }
   
}

  








    //int equal = 0;
    //for (size_t i = 0; i < knn_matches.size(); i++) {
    //    for (size_t j = 0; j < knn_matches[i].size(); j++) {
    //        DMatch match = knn_matches[i][j];
    //        if (match.queryIdx == match.trainIdx) {
    //            equal++;
    //            CHECK_EQ(match.queryIdx, match.trainIdx);
    //        }
    //        else {
    //            CHECK_NE(match.queryIdx, match.trainIdx);
    //            cout << "finish";

    //        }
    //       // CHECK(match.queryIdx == match.trainIdx);
    //        //CHECK_EQ(match.queryIdx, match.trainIdx);

    //    }
    //}
    





