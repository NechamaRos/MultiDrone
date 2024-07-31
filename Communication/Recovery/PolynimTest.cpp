//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//#include "doctest.h"
//#include "Polynom.h"
//#include <vector>
//
//TEST_CASE("Polynom class tests") {
//    vector<RS_WORD> vec = { 11, 21, 31, 14 };
//    Polynom p(vec);
//
//    SUBCASE("Constructor with vector") {
//        CHECK(p.GetCoefficient(0) == 11);
//        CHECK(p.GetCoefficient(1) == 21);
//        CHECK(p.GetCoefficient(2) == 31);
//        CHECK(p.GetCoefficient(3) == 14);
//        CHECK(p.GetCoefficient(4) == 0); // out of bounds
//    }
//
//    SUBCASE("setCopy method") {
//        vector<RS_WORD> newVec = { 1, 2, 3 };
//        p.setCopy(newVec);
//        CHECK(p.GetCoefficient(0) == 1);
//        CHECK(p.GetCoefficient(1) == 2);
//        CHECK(p.GetCoefficient(2) == 3);
//        CHECK(p.GetCoefficient(3) == 0); 
//    }
//
//    SUBCASE("setRef method") {
//        vector<RS_WORD> newVec = { 9, 8, 7 };
//        p.setRef(newVec);
//        CHECK(p.GetCoefficient(0) == 9);
//        CHECK(p.GetCoefficient(1) == 8);
//        CHECK(p.GetCoefficient(2) == 7);
//        CHECK(p.GetCoefficient(3) == 0); 
//    }
//
//    SUBCASE("print method") {
//        // You will need to manually verify the output since print method does not return a value
//        // You could redirect stdout and capture the output to verify, or simply check visually
//    }
//}
