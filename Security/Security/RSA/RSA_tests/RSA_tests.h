#ifndef RSA_TESTS
#define RSA_TESTS_H
#include "rsa.h"
#define K 25

cpp_int pollards_rho(const cpp_int& n);
int bit_length(const cpp_int& n);
bool is_key_at_least_2048_bits(cpp_int n);
std::pair<cpp_int, cpp_int> factorize(const cpp_int& n);
bool isPrime(const cpp_int& n, int k = K);


#endif
