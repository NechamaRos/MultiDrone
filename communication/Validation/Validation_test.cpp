#include "../communication/doctest.h"
#include "Validation.h"

TEST_CASE("Constructor") {
	SUBCASE("Should build the object with mat argument"){
		vector<uint8_t> key = { 1,1 };
		CHECK_NOTHROW(Validation v(key,vecValidation));
	}
	SUBCASE("Should throw error if key is empty") {
		vector<uint8_t> key = {  };
		CHECK_THROWS_AS(Validation v(key,vecValidation),invalid_argument);
	}
}
