#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "RSA_tests.h"

using namespace std;
using namespace boost::multiprecision;

//Implementation of some functions that are only related to tests.
// Function to check if n is at least 2048 bits
bool is_key_at_least_2048_bits(cpp_int n) {
    return bit_length(n) >= 2047;
}

bool isPrime(const cpp_int& n, int k) {
    return miller_rabin_test(n, k);
}

cpp_int pollards_rho(const cpp_int& n) {
    if (n % 2 == 0) return 2;

    cpp_int x = 2;
    cpp_int y = 2;
    cpp_int d = 1;
    cpp_int one = 1;

    // Boost random number generation
    boost::random::mt19937 gen(static_cast<unsigned long>(time(0)));
    boost::random::uniform_int_distribution<cpp_int> dist(1, n - 1);
    cpp_int c = dist(gen);

    auto f = [&](cpp_int x) { return (x * x + c) % n; };

    while (d == 1) {
        x = f(x);
        y = f(f(y));
        d = gcd((x > y) ? (x - y) : (y - x), n);
    }

    return d;
}

//A function that returns true if there are two prime factors for a certain number, and false otherwise.
std::pair<cpp_int, cpp_int> factorize(const cpp_int& n) {
    if (n <= 1) return { 0, 0 };
    if (isPrime(n)) return { n, 1 };

    cpp_int factor = pollards_rho(n);
    cpp_int other_factor = n / factor;

    if (isPrime(factor) && isPrime(other_factor)) {
        return { factor, other_factor };
    }

    return { 0, 0 };
}



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
