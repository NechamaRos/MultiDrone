//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//#include "doctest.h"
//#include "BWImageHandler.h"
//#include "ColorImageHandler.h"
//#include "TextHandler.h"
//#include <vector>
//#include <cstdint>
//#include <string>
//using namespace std;
//
//TEST_CASE("Convert text") {
//	TextHandler handler;
//	// טקסטים לדוגמה
//	const vector<string> messages = {
//	 "שלום עולם!",
//	 "This is a test message.",
//	 "測試文本",
//	};
//	for (const std::string& message : messages) {
//		// יצירת מערך חד-מימדי צפוי
//		std::vector<uint8_t> expectedArray;
//		for (char c : message) {
//			expectedArray.push_back(static_cast<uint8_t>(c));
//		}
//
//		std::vector<uint8_t> actualArray = handler.toOneDementionalArray(message);
//
//		CHECK(expectedArray.size() == actualArray.size());
//		for (size_t i = 0; i < expectedArray.size(); ++i) {
//			CHECK(expectedArray[i] == actualArray[i]);
//		}
//	}
//}
//
//TEST_CASE("Testing BWImageHandler::toOneDementionalArray") {
//    BWImageHandler handler;
//
//    SUBCASE("Test with 2x2 image") {
//        vector<vector<uint8_t>> imageData = {
//            {1, 2},
//            {3, 4}
//        };
//
//        vector<uint8_t> expected = { 1, 2, 3, 4 };
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//
//    SUBCASE("Test with 3x3 image") {
//        vector<vector<uint8_t>> imageData = {
//            {1, 2, 3},
//            {4, 5, 6},
//            {7, 8, 9}
//        };
//
//        vector<uint8_t> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//
//    SUBCASE("Test with empty image") {
//        vector<vector<uint8_t>> imageData = {};
//
//        vector<uint8_t> expected = {};
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//
//    SUBCASE("Test with non-square image") {
//        vector<vector<uint8_t>> imageData = {
//            {1, 2, 3},
//            {4, 5}
//        };
//
//        vector<uint8_t> expected = { 1, 2, 3, 4, 5 };
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//}
//
//
//TEST_CASE("Testing ColorImageHandler::toOneDementionalArray") {
//    ColorImageHandler handler;
//
//    SUBCASE("Test with 2x2 image") {
//        vector<vector<vector<uint8_t>>> imageData = {
//            {{1, 2, 3}, {4, 5, 6}},
//            {{7, 8, 9}, {10, 11, 12}}
//        };
//
//        vector<uint8_t> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//
//    SUBCASE("Test with 3x2 image") {
//        vector<vector<vector<uint8_t>>> imageData = {
//            {{1, 2, 3}, {4, 5, 6}},
//            {{7, 8, 9}, {10, 11, 12}},
//            {{13, 14, 15}, {16, 17, 18}}
//        };
//
//        vector<uint8_t> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//
//    SUBCASE("Test with empty image") {
//        vector<vector<vector<uint8_t>>> imageData = {};
//
//        vector<uint8_t> expected = {};
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//
//    SUBCASE("Test with non-square image") {
//        vector<vector<vector<uint8_t>>> imageData = {
//            {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},
//            {{10, 11, 12}, {13, 14, 15}}
//        };
//
//        vector<uint8_t> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//
//    SUBCASE("Test with 1x1 image") {
//        vector<vector<vector<uint8_t>>> imageData = {
//            {{42, 43, 44}}
//        };
//
//        vector<uint8_t> expected = { 42, 43, 44 };
//        vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//        CHECK(result == expected);
//    }
//
//
//    SUBCASE("Test with dynamically generated images") {
//        // Define different sizes
//        vector<pair<int, int>> sizes = { {2, 2}, {3, 2}, {1, 1}, {4, 3}, {10, 10} };
//
//        for (const auto& size : sizes) {
//            int rows = size.first;
//            int cols = size.second;
//
//            vector<vector<vector<uint8_t>>> imageData(rows, vector<vector<uint8_t>>(cols, vector<uint8_t>(3)));
//
//            // Fill the image with values
//            uint8_t value = 1;
//            for (int i = 0; i < rows; i++) {
//                for (int j = 0; j < cols; j++) {
//                    for (int k = 0; k < 3; k++) {
//                        imageData[i][j][k] = value++;
//                    }
//                }
//            }
//
//            // Create the expected result
//            vector<uint8_t> expected;
//            for (int i = 0; i < rows; i++) {
//                for (int j = 0; j < cols; j++) {
//                    for (int k = 0; k < 3; k++) {
//                        expected.push_back(imageData[i][j][k]);
//                    }
//                }
//            }
//
//            // Get the result from the function
//            vector<uint8_t> result = handler.toOneDementionalArray(imageData);
//
//            // Check if the result matches the expected
//            CHECK(result == expected);
//        }
//    }
//}
