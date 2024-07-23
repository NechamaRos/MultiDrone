
#include "doctest.h"
#include "KeyPair.h"
#include "EllipticCurve.h" 
#include"ECC.h"

TEST_CASE("KeyPair Constructor") {
    cpp_int a = 0;
    cpp_int b = 7;
    cpp_int p = cpp_int("0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    EllipticCurve curve(a, b, p);

    SUBCASE("Valid KeyPair Initialization") {
        KeyPair keyPair(curve);

        CHECK_NOTHROW(keyPair.getPrivateKey());
        CHECK_NOTHROW(keyPair.getPublicKey());
    }

    SUBCASE("Invalid KeyPair Initialization - Curve Error") {
        cpp_int a = 0;
        cpp_int b = 0;
        cpp_int p = 17;


        CHECK_THROWS_AS(EllipticCurve invalidCurve(a, b, p), std::runtime_error);
        // KeyPair keyPair(invalidCurve);
        //CHECK_THROWS_AS(keyPair, std::runtime_error);
    }

    SUBCASE("encrypt") {
        
         

        KeyPair keyPair(curve);
        ECC ecc(curve);
        const char* data = "TestData";
        size_t dataSize = strlen(data);

        CHECK_NOTHROW(ecc.encrypt(data,dataSize,keyPair.getPublicKey()));
    }
}
