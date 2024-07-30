#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "CRCalgoritm.h"
#include "../communication/doctest.h"

CRCalgoritm crc;

TEST_CASE("copy arr function") {
	SUBCASE("dest is empty") {
		vector<uint8_t> src = { 1,0,1,0,1,0 };
		vector <uint8_t>dest;
		vector<uint8_t> res = { 1,0,1,0,1,0 };

		crc.copyArr(src, dest, src.size());

		CHECK(dest == res);
	}
	SUBCASE("dest is not empty") {
		vector<uint8_t> src = { 1,0,1,0,1,0 };
		vector <uint8_t>dest = { 1,1 };
		vector<uint8_t> res = { 1,1,1,0,1,0,1,0 };

		crc.copyArr(src, dest, src.size());

		CHECK(dest == res);
	}
	SUBCASE("source is empty") {
		vector<uint8_t> src;
		vector <uint8_t>dest = { 1,1 };
		vector<uint8_t> res = { 1,1 };

		crc.copyArr(src, dest, 2);

		CHECK(dest == res);
	}
	SUBCASE("len is bigger than source") {
		vector<uint8_t> src = { 1, 2, 3 };
		vector<uint8_t> dest;

		CHECK_THROWS_AS(crc.copyArr(src, dest, 4), invalid_argument);
	}
}
TEST_CASE("getRemainder function") {

	SUBCASE("Valid key and data sizes") {
		vector<uint8_t> data = { 1, 0, 0, 1, 0,0 };
		vector<uint8_t> key = { 1,1, 0, 1 };
		vector<uint8_t> expection = { 0,0,1 };

		vector<uint8_t> res = crc.getRemainder(data, key);

		CHECK(res == expection);
	}

	SUBCASE("Invalid empty key") {
		vector<uint8_t> data = { 1, 0, 1, 1, 0 };
		vector<uint8_t> key;

		CHECK_THROWS_AS(crc.getRemainder(data, key), invalid_argument);
	}

	SUBCASE("Exception thrown during division") {
		std::vector<uint8_t> data = { 1, 0, 2, 1, 0 };
		std::vector<uint8_t> key = { 1,0,4 };

		CHECK_THROWS_AS(crc.getRemainder(data, key), runtime_error);
	}
}
TEST_CASE("receiver function") {

	SUBCASE("Valid data and key") {
		vector<uint8_t> data = { 1,0,0,1,0,0,0,0,1 };
		vector<uint8_t> key = { 1, 1,0,1 };

		CHECK_NOTHROW(crc.receiver(data, key));
	}
	SUBCASE("Invalid received data") {
		vector<uint8_t> data = { 1,0,0,0,0,0,0,0,1 };
		vector<uint8_t> key = { 1, 1,0,1 };

		CHECK_THROWS_AS_MESSAGE(crc.receiver(data, key), runtime_error, "error in received data");
	}

	SUBCASE("Invalid empty key") {
		std::vector<uint8_t> data = { 1, 0, 1, 1, 0 };
		std::vector<uint8_t> key;

		CHECK_THROWS_AS(crc.receiver(data, key), std::invalid_argument);
	}

	SUBCASE("Exception thrown during division") {
		std::vector<uint8_t> data = { 1, 0, 2, 1, 0 };
		std::vector<uint8_t> key = { 1,0,4 };

		CHECK_THROWS_AS(crc.receiver(data, key), runtime_error);
	}
}

