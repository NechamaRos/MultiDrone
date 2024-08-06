#pragma once

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace boost::multiprecision;

class RSA {

public:
	cpp_int publicKey;
	cpp_int privateKey;
	cpp_int n;


    void closeLogFile();
    int bit_length(const cpp_int& n);
    bool is_key_at_least_2048_bits(cpp_int n);
    cpp_int mod_exp(cpp_int base, cpp_int exp, cpp_int mod);
    bool miller_rabin_test(const cpp_int& n, int k);
    cpp_int generate_random_cpp_int(int bit_length);
    cpp_int generate_prime(int bit_length);
    cpp_int gcd(cpp_int a, cpp_int b);
    void set_private_and_public_keys();
    cpp_int encrypt(cpp_int  message);
    cpp_int  decrypt(cpp_int  encrpyted_text);
    vector<cpp_int> encoder(string message);
    string decoder(vector<cpp_int> encoded);
    bool isPrime(const cpp_int& n, int k = 25);
    cpp_int pollards_rho(const cpp_int& n);
    std::pair<cpp_int, cpp_int> factorize(const cpp_int& n);
};