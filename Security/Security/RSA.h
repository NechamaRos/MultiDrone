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

#define num_tests 25;

    /*cpp_int privateKey;
    cpp_int publicKey;*/
    cpp_int n;


    
    cpp_int mod_exp(cpp_int base, cpp_int exp, cpp_int mod);
    bool miller_rabin_test(const cpp_int& number_to_test, int num_tests);
    // cpp_int generate_random_cpp_int(int bit_length);
   //  cpp_int generate_prime(int bit_length);
    // cpp_int gcd(cpp_int a, cpp_int b);
    // void set_private_and_public_keys();
    cpp_int encrypt(cpp_int  message);
    cpp_int  decrypt(cpp_int  encrpyted_text);
    cpp_int convert_aes_key_to_cpp_int(const unsigned char* aes_key, size_t key_length);
    cpp_int encrypt_aes_key(const unsigned char* aes_key, size_t key_length);
    std::vector<unsigned char> decrypt_aes_key(cpp_int encrypted_aes_key);

    vector<cpp_int> encoder(string message);
    string decoder(vector<cpp_int> encoded);
};
