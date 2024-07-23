#pragma once
#include "Convert.h"
#include "../communication/doctest.h"


Convert convert;
TEST_CASE("convert char to uint8") {
	SUBCASE("Upper-case input") {
		char c = 'A';
		vector<uint8_t> expected = { 0,1,0,0 ,0,0,0,1 };

		vector<uint8_t> result = convert.convertCharToUint8(c);

		CHECK(result == expected);
	}

	SUBCASE("Lower-case input") {
		char c = 'b';
		vector<uint8_t> expected = { 0,1,1,0, 0,0,1,0 };

		vector<uint8_t> result = convert.convertCharToUint8(c);

		CHECK(result == expected);
	}

	SUBCASE("Digit input"){
		char c = '0';
		vector<uint8_t> expected = { 0,0,1,1,0,0,0,0 };

		vector<uint8_t> result = convert.convertCharToUint8(c);

		CHECK(result == expected);
	}

	SUBCASE("Number input") {
		char c = 1;
		vector<uint8_t> expected = { 0,0,0,0,0,0,0,1 };

		vector<uint8_t> result = convert.convertCharToUint8(c);

		CHECK(result == expected);
	}

	SUBCASE("zero input") {
		char c = 0;
		vector<uint8_t> expected = { 0,0,0,0,0,0,0,0 };

		vector<uint8_t> result = convert.convertCharToUint8(c);

		CHECK(result == expected);
	}

}

TEST_CASE("convert char vec to uint8") {
	SUBCASE("characters vector input") {
		vector<char> input = { 'A', 'b' };
		vector<uint8_t> expected = { 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0 };

		vector<uint8_t> result = convert.convertCharVecToUint8(input);

		CHECK(expected == result);
	}
	SUBCASE("empty vector input") {
		vector<char> input = {};
		vector<uint8_t> expected = {};
		vector<uint8_t> result = convert.convertCharVecToUint8(input);
		CHECK(expected == result);
	}
}

TEST_CASE("convert uint8 to char") {
	SUBCASE("full vector input") {
		vector<uint8_t> input = { 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0 };
		vector<char> expected = { 'A', 'b' };

		vector<char> result = convert.convertUint8ToChar(input);

		CHECK(expected == result);
	}
	SUBCASE("empty vector input") {
		vector<uint8_t> input = {};
		vector<char> expected = {};
		vector<char> result = convert.convertUint8ToChar(input);
		CHECK(expected == result);
	}
}




