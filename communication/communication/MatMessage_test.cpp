#include "doctest.h"
#include "MatMessage.h"

vector < vector<uint8_t>> mat = { {1,0,1,0,1,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1} ,{1,1,1,0,0,1} ,{1,1,0,0,1,0} };


MatMessage mm(mat);


TEST_CASE("Constructor") {
	SUBCASE("Should build the object with mat argument"){
		vector < vector<uint8_t>> mat1 = { {1,0,1,0,1,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1} ,{1,1,1,0,0,1} ,{1,1,0,0,1,0} };
		CHECK_NOTHROW(MatMessage m(mat1));
	}
	SUBCASE("Should throw error if size of the rows is not equal (it is not matrix)") {
		const vector < vector<uint8_t>> mat1 = { {1,0,1,0,1},{0,0,1,1,1,1} ,{0,0,1,0,1,1,0} ,{1,1,1,0,0,1} ,{1,1,0,0,1,0} };
		CHECK_THROWS_AS(MatMessage m(mat1), logic_error);
	}
}
TEST_CASE("CopyCol function") {
	SUBCASE("copy col from message to arr") {
		vector<uint8_t> arr;
		size_t numCol = 1;
		vector<uint8_t> expection = { 0,0,0,1,1 };

		mm.copyCol(arr, numCol);

		CHECK(arr == expection);
	}
	SUBCASE("Invalid numCol") {
		vector<uint8_t> arr;
		int numCol = 1;

		CHECK_THROWS_AS(mm.copyCol(arr, 6), invalid_argument);
	}
}
TEST_CASE("GetReminder function") {
	SUBCASE("return 3d vector of columns reminder and rows reminder") {
		vector<uint8_t> key = { 1,1,0 };

		vector<vector<vector<uint8_t>>> res = mm.getReminder(1, 1, key);

		CHECK(res[0].size() == 5);
		CHECK(res[1].size() == 6);

		CHECK(res[1][0].size() == 2);
	}
}
TEST_CASE("checkInReceive") {
	SUBCASE("The data match the data-validation") {
		vector<uint8_t> key = { 1,1,0 };
		vector<vector<vector<uint8_t>>>  validationData= mm.getReminder(1, 1, key);

		CHECK_NOTHROW(mm.checkInReceive(validationData, key));
	}
	SUBCASE("The data not match the data-validation") {
		vector<uint8_t> key = { 1,1,0 };
		vector<vector<vector<uint8_t>>>  validationData = mm.getReminder(1, 1, key);
		validationData[1][0][0] = (!validationData[1][0][0]);
		CHECK_THROWS_AS(mm.checkInReceive(validationData, key),runtime_error);
	}
}
