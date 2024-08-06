#include "doctest.h"
#include <vector>
#include <algorithm>
#include "ImageConversions.h"
using namespace std;

TEST_CASE("Simple Message Conversion") {
     vector<char> message = { 'H', 'e', 'l', 'l', 'o' };
    auto img = convertSimpleMessageToImage(message);
    auto convertedMessage = convertImageToSimpleMessage(img);
    CHECK(message == convertedMessage);
}

TEST_CASE("Black and White Image Conversion") {
     vector< vector<uint8_t>> bwImage = { {0, 255}, {255, 0} };
    auto img = convertBlackWhiteToImage(bwImage);
    auto convertedBwImage = convertImageToBlackWhite(img);
    CHECK(bwImage == convertedBwImage);
}

TEST_CASE("Color Image Conversion") {
     vector< vector< vector<uint8_t>>> colorImage = {
        {{255, 0, 0}, {0, 255, 0}},
        {{0, 0, 255}, {255, 255, 255}}
    };
    auto img = convertColorToImage(colorImage);
    auto convertedColorImage = convertImageToColor(img);
    CHECK(colorImage == convertedColorImage);
}