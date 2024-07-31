//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//#include "doctest.h"
//#include "GaloisField.h"
//#include "UseException.h"
//
//// Test for constructor and basic field properties
//TEST_CASE("GaloisField constructor") {
//    GaloisField gf(4);
//    CHECK(gf.fieldPower == 4);
//    CHECK(gf.primitivePoly == PRIM(4));
//}
//
//// Test for addition and subtraction
//TEST_CASE("GaloisField add and sub") {
//    GaloisField gf(4);
//    RS_WORD a = 5;
//    RS_WORD b = 3;
//
//    CHECK(gf.add(a, b) == (a ^ b));
//    CHECK(gf.sub(a, b) == (a ^ b));
//}
//
//// Test for multiplication without LUT
//TEST_CASE("GaloisField multNoLUT") {
//    GaloisField gf(4);
//    RS_WORD a = 2;
//    RS_WORD b = 3;
//    RS_WORD result = gf.multNoLUT(a, b);
//
//    // The expected result can be verified manually or through a known good implementation
//    CHECK(result == 6); // Example expected value, replace with actual expected value
//}
//
//// Test for multiplication with LUT
//TEST_CASE("GaloisField mult") {
//    GaloisField gf(4);
//    RS_WORD a = 2;
//    RS_WORD b = 3;
//    RS_WORD result = gf.mult(a, b);
//
//    // The expected result can be verified manually or through a known good implementation
//    CHECK(result == gf.multNoLUT(a, b)); // Example expected value, replace with actual expected value
//}
//
//// Test for inversion
//TEST_CASE("GaloisField inv") {
//    GaloisField gf(4);
//    RS_WORD a = 5;
//    RS_WORD invA = gf.inv(a);
//
//    CHECK(gf.mult(a, invA) == 1);
//}
//
//// Test for power
//TEST_CASE("GaloisField pow") {
//    GaloisField gf(4);
//    RS_WORD a = 2;
//    RS_WORD result = gf.pow(2, 3); 
//
//    // The expected result can be verified manually or through a known good implementation
//    CHECK(static_cast<int>(result) == 8); // Example expected value, replace with actual expected value
//}
//
//// Test for sqrt
//TEST_CASE("GaloisField sqrt") {
//    GaloisField gf(4);
//    RS_WORD a = 4;
//    RS_WORD result = gf.sqrt(a);
//
//    // The expected result can be verified manually or through a known good implementation
//    CHECK(result == 2); // Example expected value, replace with actual expected value
//}
//
//// Test edge cases
//TEST_CASE("GaloisField edge cases") {
//    GaloisField gf(4);
//
//    // Test zero multiplication
//    CHECK(gf.mult(0, 5) == 0);
//    CHECK(gf.mult(5, 0) == 0);
//
//    // Test one multiplication
//    CHECK(gf.mult(1, 5) == 5);
//    CHECK(gf.mult(5, 1) == 5);
//
//    // Test self division
//    CHECK(gf.div(5, 5) == 1);
//
//    // Test max values
//    RS_WORD maxValue = (1 << gf.fieldPower) - 1; // 255 for GF(2^8)
//    CHECK(gf.add(maxValue, 0) == maxValue);
//    CHECK(gf.sub(maxValue, 0) == maxValue);
//    CHECK(gf.mult(maxValue, 1) == maxValue);
//    CHECK(gf.div(maxValue, 1) == maxValue);
//
//    // Test inversion
//    for (RS_WORD i = 1; i <= maxValue; ++i) {
//        CHECK(gf.mult(i, gf.inv(i)) == 1);
//    }
//
//    // Test invalid values (assuming the GF class handles out of range values gracefully)
//    CHECK_THROWS(gf.mult(254, 5));
//    CHECK_THROWS(gf.mult(254, 5));
//    CHECK_THROWS(gf.div(254, 5));
//    CHECK_THROWS(gf.add(254, 5));
//    CHECK_THROWS(gf.sub(254, 5));
//}
