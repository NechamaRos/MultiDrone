#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "HeaderFile.H";

TEST_CASE("Testing of not valid input")
{
	//CHECK_THROWS_AS(homography({ MPoint(150, 150), MPoint(250, 150) }, { MPoint(250, 250), MPoint(150, 250) }, 3), std::runtime_error);
	//CHECK_THROWS_WITH_AS(homography({ {150, 150},{250, 150} }, { {250, 250},{150, 250} }, 3), "It is impossible to do a Homography with less than 4 Points", std::runtime_error);
	//CHECK_THROWS_AS(homography({ {150, 150},{250, 150},{100,100},{505,242},{42,300} }, { {250, 250},{150, 250},{100,150}, {250,300} }, 3), std::runtime_error);
	vector<double> vd(9);
	CHECK(homography({ {150, 150},{250, 150} }, { {250, 250},{150, 250} }, 3) == vd);
	CHECK(homography({{ 150, 150 }, { 250, 150 }, { 100,100 }, { 505,242 }, { 42,300 }}, { {250, 250},{150, 250},{100,150}, {250,300} }, 3) == vd);

}
//
//TEST_CASE("good homography matrix for 4 points in vectors- like OpenCV")
//{
//	int a1x = 100, a1y = 202;
//	int b1x = 350, b1y = 701;
//	int c1x = 170, c1y = 300;
//	int d1x = 120, d1y = 400;
//	
//	int a2x = 150, a2y = 680;
//	int b2x = 250, b2y = 150;
//	int c2x = 500, c2y = 240;
//	int d2x = 180, d2y = 320;
//
//
//	vector<Point2f> points1 = { { static_cast<float>(a1x), static_cast<float>(a1y)},{static_cast<float>(b1x),static_cast<float>(b1y)},{static_cast<float>(c1x), static_cast<float>(c1y)},{static_cast<float>(d1x),static_cast<float>(d1y)} };
//	vector<Point2f> points2 = { { static_cast<float>(a2x), static_cast<float>(a2y)},{static_cast<float>(b2x),static_cast<float>(b2y)},{static_cast<float>(c2x), static_cast<float>(c2y)},{static_cast<float>(d2x),static_cast<float>(d2y)} };
//	vector<MPoint> v1 = { {a1x, a1y},{b1x,b1y},{c1x, c1y},{d1x,d1y} };
//	vector<MPoint> v2 = { {a2x, a2y},{b2x,b2y},{c2x, c2y},{d2x,d2y} };
//
//	cv::Mat H = cv::findHomography(points1, points2);
//	vector<double> myH = (homography(v1, v2));
//	
//	CHECK(H.rows * H.cols == myH.size());
//
//	cv::Mat H2 = cv::findHomography(points1, points2, cv::RANSAC, 3);
//
//
//	int a = 0;
//	for (int i = 0; i <H.rows; ++i)
//		for (int j = 0; j < H.cols; j++)
//		{
//			double s = H.at<double>(i, j)- myH.at(a++) ;
//			//================
//			CHECK(s < DBL_EPSILON);
//		}
//}

TEST_CASE("print an empty matrix if the point is colinary")
{
	vector<MPoint> v1 = { {100, 100},{200,200},{300, 300},{400,400},{500,500} };
	vector<MPoint> v2 = { {105, 98},{250,320},{410, 120},{520,120},{360,266} };
	vector<double> myH = (homography(v1, v2));
	for (int i = 0; i < myH.size(); i++)
	{
		CHECK(myH.at(i) == 0);
	}
}
TEST_CASE("print an empty matrix if can not calculate the homography #2")
{
	vector<MPoint> v1 = { {100, 100},{200,200},{300, 300},{400,400},{500,500} };
	vector<MPoint> v2 = { {1, 2},{1,5},{4, 8},{12,7},{4,2} };
	vector<double> myH = (homography(v1, v2));
	for (int i = 0; i < myH.size(); i++)
	{
		CHECK(myH.at(i) == 0);
	}

}
TEST_CASE("check if the number of the good point of the homography same as OpenCV")
{

	vector<Point2f> points1 = {
	{150, 150}, {180, 170}, {220, 160}, {250, 200},
	{100, 100}, {200, 100}, {200, 200}, {100, 200},
	{130, 180}, {180, 250}, {150, 680}, {250, 150},
	{500, 240}
	};
	vector<Point2f> points2 = {
	{170, 160}, {200, 170}, {230, 180}, {260, 210},
	{150, 150}, {250, 150}, {250, 250}, {150, 250},
	{140, 190}, {190, 260}, {100, 202}, {350, 701},
	{170, 300}
	};
	Mat mask;
	cv::Mat H = cv::findHomography(points1, points2, cv::RANSAC, 3,mask);
	int goodPointOpCv = 0;
	int myGoodPoint = 0;
	for (size_t i = 0; i < points1.size(); i++) 
		if (mask.at<uchar>(i)) {
			goodPointOpCv++;
		}
}

TEST_CASE("test of the function- 'checkIfOnSameLine' ")
{
	vector<MPoint> v1 = { {100, 100},{200,200},{300, 300},{400,400},{500,500} };
	vector<MPoint> v2 = { {105, 98},{250,320},{410, 120},{520,120},{360,266} };
	CHECK(checkIfOnSameLine(v1) == 1);
	CHECK(checkIfOnSameLine(v2) == 0);
}
TEST_CASE("equal two matrix and what is more good")
{
	std::vector<MPoint> points1 = {
		{100, 100}, {150, 150}, {200, 200}, {250, 250}, {300, 300},
		{105, 100}, {155, 150}, {205, 200}, {255, 250}, {305, 300}, 
		{400, 500}, {350, 450}, {250, 400}, {200, 350}, {150, 300}  
	};

	std::vector<MPoint> points2 = {
		{110, 110}, {160, 160}, {210, 210}, {260, 260}, {310, 310},
		{115, 110}, {165, 160}, {215, 210}, {265, 260}, {315, 310},
		{410, 510}, {360, 460}, {260, 410}, {210, 360}, {160, 310} 
	};
	vector<double> lessGood = { -0.57806453892815357,1.5564472164770484 ,17.566062857877103 ,
	-1.9887936654984586 ,2.9671763430473534 ,19.727795102984150 ,
	-0.0043234644902141505 ,0.0043234644902140637, 1.0000000000000000 };
	vector<double> moreGood = { 1, 1.67745e-14, 10,
	-6.93001e-15, 1, 10,
	-4.35443e-17, 7.82529e-17, 1 };

	int cl= checkIfBetter(points1, points2, lessGood,3);
	int cm = checkIfBetter(points1, points2, moreGood, 3);
	CHECK(cm > cl);

}
