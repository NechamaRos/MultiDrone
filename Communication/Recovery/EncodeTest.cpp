#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ReedSoloEncode.h"
#include "Meta_Data.h"
#include "UsePoly.h"
#include "GaloisField.h"
#include "Data.h"

TEST_SUITE("ReedSoloEncode Tests") {

    TEST_CASE("createGenerator") {
        int fieldPower = 4;
        ReedSoloEncode rs(fieldPower);
        Polynom generator;
        int nsym = 2;

        rs.createGenerator(&generator, nsym);

        CHECK(generator.coef.size() == nsym + 1); // Check size of generator polynomial
        CHECK(generator.coef[0] == 1); // First coefficient should be 1

        // Further checks can be added to verify the content of the generator polynomial
    }

    TEST_CASE("encode") {
        int fieldPower = 4;
        ReedSoloEncode rs(fieldPower);
        vector<RS_WORD> out;
        vector<RS_WORD> data = { 1, 2, 3, 4 };
        int nsym = 2;

        rs.encode(out, data, nsym);

        CHECK(out.size() == data.size() + nsym); // Check size of the output

        // Further checks can be added to verify the correctness of the encoded data
    }
}

TEST_CASE("main Function Test") {
    ReedSoloEncode rs(2);
    vector<RS_WORD> out;
    vector<RS_WORD> data = { 1, 2, 3, 4 };
    rs.encode(out, data, 2);

    CHECK(out.size() == data.size() + 2); // Check size of the output

    // Further checks can be added to verify the correctness of the encoded data
}
