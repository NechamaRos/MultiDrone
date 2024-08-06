#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "C:\Users\The user\Desktop\מסמכים לימודיים\פרקטיקום\RSAAlgorithem_after_boost\RSAAlgorithem_after_boost\doctest.h"
#include "rsa.h"

using namespace std;
using namespace boost::multiprecision;
// Test cases
TEST_CASE("testing gcd function") {
    CHECK(gcd(48, 18) == 6);
    CHECK(gcd(101, 103) == 1);
    CHECK(gcd(54, 24) == 6);
}
TEST_CASE("testing if RSA key length is at least 2048 bits") {
    std::cout << "Setting private and public keys..." << std::endl;
    set_Private_And_Public_Keys();
    cout << "the num of bits in n:" << endl;
    cout << bit_length(n) << endl;
    std::cout << "Checking if key is at least 2048 bits..." << std::endl;
    CHECK(is_key_at_least_2048_bits(n));
}

TEST_CASE("testing encryption and decryption of a message with special characters") {
    set_Private_And_Public_Keys();
    string original = "H3ll0, RSA! @2024";
    auto encoded = encoder(original);
    auto decoded = decoder(encoded);
    CHECK(decoded == original);
}
TEST_CASE("testing empty message encryption and decryption") {
    set_Private_And_Public_Keys();
    string original = "";
    auto encoded = encoder(original);
    auto decoded = decoder(encoded);
    CHECK(decoded == original);
}
TEST_CASE("testing encryption and decryption of a message with special characters") {
    set_Private_And_Public_Keys();
    string original = "H3ll0, RSA! @2024";
    auto encoded = encoder(original);
    auto decoded = decoder(encoded);
    CHECK(decoded == original);
}
TEST_CASE("A test that checks whether the keys can be hacked.") {
    set_Private_And_Public_Keys();
    std::pair<cpp_int, cpp_int> factors = factorize(n);
    // Check if the factorization does not find valid factors
    CHECK(factors == std::make_pair(cpp_int(0), cpp_int(0)));
}
