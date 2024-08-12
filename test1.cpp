#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world4100d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world4100.lib")
#endif

#include "doctest.h"
#include "Image.h"
#include <tuple>
#include <opencv2/opencv.hpp>
#define _CRTDBG_MAP_ALLOC //to get more details
#include <stdlib.h>  
#include <crtdbg.h>   //for malloc and free

using namespace std;
using namespace cv;

TEST_CASE("check the memory management")
{
    _CrtMemState sOld;
    _CrtMemState sNew;
    _CrtMemState sDiff;

    _CrtMemCheckpoint(&sOld);//save the memory state
    
    {
        Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
        auto data = img.getData();
        Image img2(img.width, img.height, 3, data);
        Image img3(3, 3, 3);
        Image img4 = img;
        Image img5(img);
    }

    _CrtMemCheckpoint(&sNew);//save the new memory state

    //if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
    //{
    //    OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
    //    _CrtMemDumpStatistics(&sDiff);
    //    OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
    //    _CrtMemDumpAllObjectsSince(&sOld);
    //    OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
    //    _CrtDumpMemoryLeaks();
    //}

    CHECK(!_CrtMemDifference(&sDiff, &sOld, &sNew));// if there is a difference
}

TEST_CASE("check the readImage function with invalid argumant")
{
    CHECK_THROWS_AS(Image::readImage("rest8.BMP"), runtime_error);
}

TEST_CASE("check the readImage function")
{
    Mat m = imread("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK(img.width == m.cols);
    CHECK(img.height == m.rows);
    bool same = true;
    for (int i = 0; i < img.width * img.height * img.channels; i++)
    {
        if (img.data[i] != m.data[i])
            same = false;
    }
    CHECK(same==true);
}

TEST_CASE("check the readImage function with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK_THROWS_AS(Image::writeImage("C:\\Users\\The user\\Desktop\\aaaaaa\\rest8.BMP",img), runtime_error);
}

TEST_CASE("check the writeImage function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image::writeImage("tryWriteImage.bmp", img);
    Image img2 = Image::readImage("tryWriteImage.bmp");

    CHECK(img.width == img2.width);
    CHECK(img.height == img2.height);
    bool same = true;
    for (int i = 0; i < img.width * img.height * img.channels; i++)
    {
        if (img.data[i] != img2.data[i])
            same = false;
    }
    CHECK(same == true);
}

//TEST_CASE("check the resize function")
//{
//    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
//    Image img2 = Image::resize(img,make_tuple(200,300));
//    Mat img3 = imread("C:\\Users\\The user\\Desktop\\rest8.BMP");
//    Mat resizedImage;
//    Size newSize(200,300); // Set the new size (width, height)
//    resize(img3, resizedImage, newSize);
//
//    bool same = true;
//
//    for (int y = 0; y < resizedImage.rows; y++) 
//    {
//        for (int x = 0; x < resizedImage.cols; x++) 
//        {
//            Vec3b pixel = resizedImage.at<cv::Vec3b>(y, x);
//            if (pixel[0] != img2.data[y * img2.width * 3 + x])
//                same = false;
//            if (pixel[1] != img2.data[y * img2.width * 3 + x+1])
//                same = false;
//            if (pixel[2] != img2.data[y * img2.width * 3 + x+2])
//                same = false;
//        }
//    }
//
//    CHECK(same == true);
//}

TEST_CASE("check the total function") 
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK(img.total() == 201588);
}

TEST_CASE("check the getSize function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK(img.getSize() == std::make_tuple(314, 214));
}

TEST_CASE("check the at function that return a specific channel with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK_THROWS_AS(img.at(215, 1, 1), std::out_of_range);
    CHECK_THROWS_AS(img.at(1, 314, 1), std::out_of_range);
    CHECK_THROWS_AS(img.at(1, 1, 3), std::out_of_range);
}

TEST_CASE("check the at function that return a specific channel")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK(img.at(1, 1, 1) == 230);
    CHECK(img.at(213, 313, 2) == 38);
    CHECK(img.at(100,100,1) == 121);
}

TEST_CASE("check the at function that return the BGR of the pixel with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK_THROWS_AS(img.at(215, 1), out_of_range);
    CHECK_THROWS_AS(img.at(1, 314), out_of_range);
}

TEST_CASE("check the at function that return the BGR of the pixel")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    vector<uint8_t>v{ 254,225,233 };
    vector<uint8_t>v2{ 42,36,38 };
    vector<uint8_t>v3{ 62,51,50 };
    auto res = img.at(0, 0);
    auto res2 = img.at(213, 313);
    auto res3 = img.at(103,145);
    CHECK(res == v);
    CHECK(res2 == v2);
    CHECK(res3 == v3);
}

TEST_CASE("check the operator== with other Image, with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2 = Image::resize(img, make_tuple(100, 100));
    Image img3 = Image::resize(img, make_tuple(214, 100));
    CHECK_THROWS_AS(img == img2, exception);
    CHECK_THROWS_AS(img==img3,exception);
}

TEST_CASE("check the operator== with other Image")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2= Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img3= Image::readImage("C:\\Users\\The user\\Desktop\\rest10.BMP");
    Image img4 = Image::resize(img3, make_tuple(img.width, img.height));

    bool isSame = img == img2;
    CHECK(isSame==true);

    isSame = img == img4;
    CHECK(isSame != true);
}

TEST_CASE("check the operator== with value")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
  
    bool isSame=img == 3;
    bool res = true;
    
    for (int i = 0; i < img.width * img.height * img.channels; i++)
    {
        if (img.data[i] != 3)
            res = false;
    }
    CHECK(isSame == res);
}

TEST_CASE("check the operator+ with value, with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK_THROWS_AS(img + 255, invalid_argument);
}

TEST_CASE("check the operator+ with value")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");

    Image img2 = img + 0;
    bool res = true;

    for (int i = 0; i < img.width * img.height * img.channels; i++)
    {
        if (img.data[i]+0 != img2.data[i])
            res = false;
    }
    CHECK(res == true);
}

TEST_CASE("check the operator- with value with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    CHECK_THROWS_AS(img - 444, invalid_argument);
}

TEST_CASE("check the operator- with value")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");

    Image img2 = img - 3;
    bool res = true;

    for (int i = 0; i < img.width * img.height * img.channels; i++)
    {
        if (img.data[i] - 3 != img2.data[i])
            res = false;
    }
    CHECK(res == true);
}

TEST_CASE("check the operator= with other image")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    int refCount = *(img.refCount);
    Image img2 = img;
 
    CHECK(img.data == img2.data);
    CHECK(img.width == img2.width);
    CHECK(img.height == img2.height);
    CHECK(img.ROI == img2.ROI);
    CHECK(img.channels == img2.channels);
    CHECK(img.refCount == img2.refCount);
    CHECK(*(img.refCount) == refCount+1);
}

TEST_CASE("check the operator+ with other image, with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2 = Image::readImage("C:\\Users\\The user\\Desktop\\rest10.BMP");
    CHECK_THROWS_AS(img + img2, invalid_argument);
}

TEST_CASE("check the operator+ with other image in case they both have the same width")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2 = Image::readImage("C:\\Users\\The user\\Desktop\\rest10.BMP");
    Image img3 = Image::resize(img2, make_tuple(img.width, img2.height));
    Image resImage = img + img3;
    bool res = true;
    int imgSize = img.total();
    int resImgSize = resImage.total();
    for (int i = 0; i < resImgSize; i++)
    {
        if (i < imgSize)
        {
            if (resImage.data[i] != img.data[i])
                res = false;
        }
        else
        {
            if (resImage.data[i] != img3.data[i-imgSize])
                res = false;
        }
      
    }
    CHECK(res == true);
}

TEST_CASE("check the operator+ with other image in case they both have the same height")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2 = Image::readImage("C:\\Users\\The user\\Desktop\\rest10.BMP");
    Image img3 = Image::resize(img2, make_tuple(img2.width, img.height));
    Image resImage = img + img3;
    bool res = true;
    int imgSize = img.total();
    int resImgSize = resImage.total();
    int imgRowSize = img.width * 3;
    for (int i = 0; i < resImage.height; i++)
    {
        int toThisLine = i * resImage.width * 3;
        for (int j = 0; j < img.width*3; j++)
        {          
            if (resImage.data[toThisLine +j] != img.data[i*img.width*3+j])
                res = false;
        }
        for (int j = 0; j < img3.width * 3; j++)
        {           
            if (resImage.data[toThisLine+img.width*3 + j] != img3.data[i * img3.width*3 + j])
                res = false;          
        }
    }
    CHECK(res == true);
}

TEST_CASE("check the clone function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2 = img.clone();

    CHECK(img.data != img2.data);
    CHECK(img.width == img2.width);
    CHECK(img.height == img2.height);
    CHECK(img.ROI == img2.ROI);
    CHECK(img.channels == img2.channels);
    CHECK(img.refCount != img2.refCount);
    bool res = true;

    for (int i = 0; i < img.width * img.height * img.channels; i++)
    {
        if (img.data[i] != img2.data[i])
            res = false;
    }
    CHECK(res == true);
}

TEST_CASE("check the empty function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2(5, 5);
    Image img3;

    CHECK(img.empty() == false);
    CHECK(img2.empty() == true);
    CHECK(img3.empty() == true);
}

TEST_CASE("check the getData function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    auto v = img.getData();
    bool res = true;
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] != img.data[i])
            res = false;
    }
    CHECK(res== true);   
}

TEST_CASE("check the splitChannels function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Mat img2 = imread("C:\\Users\\The user\\Desktop\\rest8.BMP");
    auto v = img.splitChannels();
    vector<Mat> channels;
    split(img2, channels);
    bool res = true;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < v[0].size(); j++)
        {
            if (v[i][j] != channels[i].data[j])
                res = false;
        }
    }
    CHECK(res == true);
}

TEST_CASE("check the margeChannels function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    auto v = img.splitChannels();
    auto v2 = Image::margeChannels(v);
    bool res = true;
    for (int i = 0; i < v2.size(); i++)
    {
        if (v2[i] != img.data[i])
            res = false;
    }
    CHECK(res == true);
}

TEST_CASE("check the setAllPixel function with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    vector<uint8_t>pixel = { 122,111,145 };

    CHECK_THROWS_AS(img.setAllPixel(1, 314, pixel), invalid_argument);
    CHECK_THROWS_AS(img.setAllPixel(214,1,pixel), invalid_argument);
}

TEST_CASE("check the setAllPixel function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2 = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    vector<uint8_t>pixel = { 255,0,0 };

    img.setAllPixel(1, 1, pixel);
    img.setAllPixel(213, 313, pixel);
    img.setAllPixel(92, 39, pixel);
    
    auto v = img.at(1, 1);
    auto v2=img2.at(1, 1);
    bool same = v == v2;
    bool sameV = v==pixel;
    CHECK(same != true);
    CHECK(sameV==true);

    v = img.at(213, 313);
    v2 = img2.at(213, 313);
    same = v == v2;
    sameV = v == pixel;
    CHECK(same != true);
    CHECK(sameV == true);

    v = img.at(92, 39);
    v2 = img2.at(92, 39);
    same = v == v2;
    sameV = v == pixel;
    CHECK(same != true);
    CHECK(sameV == true);
}

TEST_CASE("check the setPixelCH function with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");

    CHECK_THROWS_AS(img.setPixelCH(1, 314, 3,2), invalid_argument);
    CHECK_THROWS_AS(img.setPixelCH(214, 1, -1, 3), invalid_argument);
    CHECK_THROWS_AS(img.setPixelCH(214, 1, 1, -3), invalid_argument);
    CHECK_THROWS_AS(img.setPixelCH(214, 500, 1, -3), invalid_argument);
    CHECK_THROWS_AS(img.setPixelCH(-214, 1, 1,-3), invalid_argument);
}

TEST_CASE("check the setPixelCH function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");
    Image img2 = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");

    img.setPixelCH(1, 1, 0, 0);
    img.setPixelCH(1, 1, 1, 0);
    img.setPixelCH(1, 1, 2, 255);
  
    img.setPixelCH(95, 95, 0, 0);
    img.setPixelCH(95, 95, 1, 0);
    img.setPixelCH(95, 95, 2, 255);

    auto v = img.at(1, 1,1);
    auto v2 = img2.at(1, 1,1);
    bool same = v == v2;
    bool sameC = v ==0;

    CHECK(same != true);
    CHECK(sameC == true);

    v = img.at(95, 95,2);
    v2 = img2.at(95, 95,2);
    same = v == v2;
    sameC = v == 255;
    CHECK(same != true);
    CHECK(sameC == true);
}

TEST_CASE("check the row function with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");

    CHECK_THROWS_AS(img.row(500), invalid_argument);
}

TEST_CASE("check the row function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");

    Image img2 = img.row(100);
  
    bool same = true;

    for (int i = 0; i < img.width*3; i++)
    {
        if (img.data[img.width*3*100+i] != img2.data[i])
            same = false;
    }
   
    CHECK(same == true);
}

TEST_CASE("check the col function with invalid argumant")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");

    CHECK_THROWS_AS(img.col(314), invalid_argument);
}

TEST_CASE("check the col function")
{
    Image img = Image::readImage("C:\\Users\\The user\\Desktop\\rest8.BMP");

    Image img2 = img.col(100);

    bool same = true;

    for (int i = 0; i < img2.height; i++)
    {
        auto pixel = img.at(i, 100);
        auto pixel2 = img2.at(i,0);
        if (pixel == pixel2)
            continue;
        else
            same = false;
    }

    CHECK(same == true);
}

//test on the ptr functions, resize waitKey