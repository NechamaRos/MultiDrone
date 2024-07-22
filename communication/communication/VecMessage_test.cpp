#include "doctest.h"
#include "VecMessage.h"

vector<char> message = { 'I',' ','r','a','n',' ','o','u','t',' ','o','f',' ','b','a','t','t','e','r','y' };
VecMessage vm(message);

TEST_CASE("getremeinder function") {

    SUBCASE("get the remeinder") {
        vector<uint8_t> key = { 1, 0, 1 };
        vector<uint8_t> expection = { 1, 1 };

        vector<uint8_t> res = vm.getreminder(0, key);

        CHECK(expection == res);
    }

    SUBCASE("Exception thrown from crc.getRemeinder") {
        vector<uint8_t> key ;

        CHECK_THROWS_AS(vm.getreminder(10, key), invalid_argument);

        key = { 3,2,1 };

        CHECK_THROWS_AS(vm.getreminder(10, key), runtime_error);
    }
}
TEST_CASE("checkInReceive function") {

    SUBCASE("The data match the data-validation") {
        vector<uint8_t> key = { 1, 0, 1 };
        vector<uint8_t> validation = { 1, 1 };

        CHECK_NOTHROW(vm.checkInReceive(validation,key));
    }

    SUBCASE("The data not match the data-validation") {
        vector<uint8_t> key = { 1, 0, 1 };
        vector<uint8_t> validation = { 1, 0 };

        CHECK_THROWS_AS(vm.checkInReceive(validation, key),runtime_error);
    }
}