#include "../Communication/doctest.h"
#include "ImageConversions.h"
#include <algorithm>
using namespace std;

TEST_CASE("Image Conversions Tests") {

    SUBCASE("Test simple Message Conversion") {
        vector<char> message = { 'H', 'e', 'l', 'l', 'o' };
        auto img = convertSimpleMessageToImage(message);
        auto convertedMessage = convertImageToSimpleMessage(img);
        CHECK(message == convertedMessage);
    }

    SUBCASE("Test simple message conversion with special characters") {
        vector<char> message = { 'H', 'e', 'l', 'l', 'o', '!', '\n', '\t', '\0' };
        auto img = convertSimpleMessageToImage(message);
        auto convertedMessage = convertImageToSimpleMessage(img);
        CHECK(message == convertedMessage);
    }

    SUBCASE("Test black and White Image Conversion") {
        vector< vector<uint8_t>> bwImage = { {0, 255}, {255, 0} };
        auto img = convertBlackWhiteToImage(bwImage);
        auto convertedBwImage = convertImageToBlackWhite(img);
        CHECK(bwImage == convertedBwImage);
    }

    SUBCASE("Test black and white image conversion with checkerboard pattern") {
        vector<vector<uint8_t>> checkerboard = {
            {0, 255, 0, 255},
            {255, 0, 255, 0},
            {0, 255, 0, 255},
            {255, 0, 255, 0}
        };
        auto img = convertBlackWhiteToImage(checkerboard);
        auto convertedBwImage = convertImageToBlackWhite(img);
        CHECK(checkerboard == convertedBwImage);
    }

    SUBCASE("Test color Image Conversion") {
        vector< vector< vector<uint8_t>>> colorImage = {
           {{255, 0, 0}, {0, 255, 0}},
           {{0, 0, 255}, {255, 255, 255}}
        };
        auto img = convertColorToImage(colorImage);
        auto convertedColorImage = convertImageToColor(img);
        CHECK(colorImage == convertedColorImage);
    }

    SUBCASE("Test color image conversion with RGB color wheel") {
        vector<vector<vector<uint8_t>>> colorWheel = {
            {{255, 0, 0}, {255, 127, 0}, {255, 255, 0}},
            {{0, 255, 0}, {0, 255, 255}, {0, 0, 255}},
            {{127, 0, 255}, {255, 0, 255}, {255, 255, 255}}
        };
        auto img = convertColorToImage(colorWheel);
        auto convertedColorImage = convertImageToColor(img);
        CHECK(colorWheel == convertedColorImage);
    }

    SUBCASE("Test conversion with empty images") {
        vector<vector<uint8_t>> emptyBW;
        auto imgBW = convertBlackWhiteToImage(emptyBW);
        auto convertedBW = convertImageToBlackWhite(imgBW);
        CHECK(emptyBW == convertedBW);

        vector<vector<vector<uint8_t>>> emptyColor;
        auto imgColor = convertColorToImage(emptyColor);
        auto convertedColor = convertImageToColor(imgColor);
        CHECK(emptyColor == convertedColor);
    }

    SUBCASE("Test bit string and byte vector conversions") {
        string bitString = "1010101010101010";
        auto bytes = bitStringToBytes(bitString);
        auto convertedBitString = bytesToBitString(bytes);
        CHECK(bitString == convertedBitString);

        vector<unsigned char> byteVector = { 0xAA, 0x55, 0xFF, 0x00 };
        auto convertedString = bytesToBitString(byteVector);
        auto convertedBytes = bitStringToBytes(convertedString);
        CHECK(byteVector == convertedBytes);
    }
}
