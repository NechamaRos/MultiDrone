#include "doctest.h"
#include "D3Message.h"

vector < vector<vector<uint8_t>>> d3mat = { { {1,0,1,0,1,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1} ,{1,1,1,0,0,1} ,{1,1,0,0,1,0} } ,
{ {1,0,1,0,1,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1} ,{1,1,1,0,0,1} ,{1,1,0,0,1,0} } ,
{ {1,0,1,0,1,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1} ,{1,1,1,0,0,1} ,{1,1,0,0,1,0} } };


D3Message mm3(d3mat);

TEST_CASE("Constructor") {
	SUBCASE("Should build the object with mat argument") {
		CHECK_NOTHROW(D3Message m(d3mat));
	}
	SUBCASE("Should throw error if message is not include equal size matrixes") {
		vector < vector<vector<uint8_t>>> d3mat1 = { { {1,0,1,0,1,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1} ,{1,1,1,0,0,1} ,{1,1,0,0,1,0} } ,
		{ {1,0,1,0,1,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1} ,{1,1,1,0,0,1} , } ,
		{ {1,0,1,0,1,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1} ,{1,1,1,0,0,1} ,{1,1,0,0,1,0} } };
		CHECK_THROWS_AS(D3Message m(d3mat1), logic_error);
	}
}
TEST_CASE("GetReminder function") {
	SUBCASE("return 4d vector of columns reminder and rows remainder") {
		vector<uint8_t> key = { 1,1,0 };

		vector<vector<vector<vector<uint8_t>>>> res = mm.getReminder(1, 1,1, key);

		CHECK(res[0].size() == 2);
		CHECK(res[0][1].size() == 6);

		CHECK(res.size() == 3);
	}
}
TEST_CASE("checkInReceive") {
	SUBCASE("The data match the data-validation") {
		vector<uint8_t> key = { 1,1,0 };
		vector<vector<vector<vector<uint8_t>>>>  validationData = mm.getReminder(1, 1,1, key);

		CHECK_NOTHROW(mm3.checkInReceive(validationData, key));
	}
	SUBCASE("The data not match the data-validation") {
		vector<uint8_t> key = { 1,1,0 };
		vector<vector<vector<vector<uint8_t>>>>  validationData = mm.getReminder(1,1, 1, key);
		validationData[0][1][0][0] = (!validationData[0][1][0][0]);
		CHECK_THROWS_AS(mm3.checkInReceive(validationData, key), runtime_error);
	}
}
