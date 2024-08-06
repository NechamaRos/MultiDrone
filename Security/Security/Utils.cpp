#include "Utils.h"
#include <cstdlib>
#include <algorithm> // Include the appropriate header
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
using namespace std;
using namespace boost::multiprecision;

cpp_int generateSecureRandomNumber(const cpp_int& min, const cpp_int& max) {
    std::random_device rd; // To obtain a random seed
    std::mt19937_64 gen(rd()); // Mersenne Twister generator

    // Number of bytes required
    size_t numBytes = (max.backend().size() * sizeof(uint64_t));

    std::vector<uint8_t> bytes(numBytes);

    // Generate random bytes
    for (size_t i = 0; i < numBytes; ++i) {
        bytes[i] = static_cast<uint8_t>(gen());
    }

    // Convert bytes to cpp_int
    cpp_int randomNumber;
    for (size_t i = 0; i < numBytes; ++i) {
        randomNumber <<= 8;
        randomNumber += bytes[i];
    }

    // Ensure the number is within the range [min, max]
    cpp_int range = max - min + 1;
    randomNumber = min + (randomNumber % range);

    return randomNumber;
}
// Function to compute the modulo (remainder) of two numbers
cpp_int mod(cpp_int a, cpp_int b) {
    if (b <= 0) {
        throw std::invalid_argument("Modulus must be positive");
    }
    cpp_int result = a % b;
    if (result < 0) result += b;
    return result;
}

// Function to compute the modular inverse of a number
cpp_int modInverse(cpp_int a, cpp_int m) {
    cpp_int m0 = m, t, q;
    cpp_int x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

// Function to compute the greatest common divisor (GCD) of two numbers
cpp_int gcd(cpp_int a, cpp_int b) {
    while (b != 0) {
        cpp_int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to generate a random private key in the range [1, n-1]
cpp_int generateRandomPrivateKey(cpp_int n) {
    return generateSecureRandomNumber(1, n);
}
