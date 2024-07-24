
#include "doctest.h"
#include "EllipticCurve.h"
#include"ECC.h"
#include "KeyPair.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

TEST_CASE("ECC Encryption and Decryption Tests") {
    
    const char* data = "test data";

    EllipticCurve curve;
    ECC ecc(curve);

    KeyPair keyPair(curve);

    SUBCASE("Encryption Test") {
        EncryptionResult result = ecc.encrypt(data, keyPair.getPublicKey());

        REQUIRE_FALSE(result.ciphertext.empty());
        REQUIRE_FALSE(result.R.isInfinity());
    }

    SUBCASE("Decryption Test") {
        EncryptionResult result = ecc.encrypt(data, keyPair.getPublicKey());
        char* decryptedData = ecc.decrypt<char>(result, keyPair.getPrivateKey());

        REQUIRE(decryptedData != nullptr);
        REQUIRE(*decryptedData == *data);

        delete[] decryptedData; // Clean up
    }
}