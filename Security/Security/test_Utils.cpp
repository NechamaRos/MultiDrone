#include "doctest.h"
#include "Utils.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

TEST_CASE("Testing mod function") {
    CHECK(mod(10, 3) == 1);
    CHECK(mod(-10, 3) == 2);
    //CHECK(mod(10, -3) == 1);
    //CHECK(mod(-10, -3) == 2);
}

TEST_CASE("Testing modInverse function") {
    CHECK(modInverse(3, 11) == 4);
    CHECK(modInverse(10, 17) == 12);
    CHECK(modInverse(10, 1) == 0);
}

TEST_CASE("Testing gcd function") {
    CHECK(gcd(54, 24) == 6);
    CHECK(gcd(48, 18) == 6);
    CHECK(gcd(101, 103) == 1);
}

TEST_CASE("Testing generateRandomPrivateKey function") {
    cpp_int n("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
    for (int i = 0; i < 100; ++i) {
        cpp_int key = generateRandomPrivateKey(n);
        CHECK(key >= 1);
        CHECK(key < n);
    }
}