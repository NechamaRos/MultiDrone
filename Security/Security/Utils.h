#ifndef UTILS_H
#define UTILS_H
#include <boost/multiprecision/cpp_int.hpp> 
using namespace boost::multiprecision;
#include <vector>
#include <string>

// הכרזה על הפונקציה

cpp_int generateSecureRandomNumber(const cpp_int& min, const cpp_int& max);
cpp_int mod(cpp_int a, cpp_int b);
cpp_int modInverse(cpp_int a, cpp_int b);
cpp_int gcd(cpp_int a, cpp_int b);
cpp_int generateRandomPrivateKey(cpp_int p);

#endif // UTILS_H
 