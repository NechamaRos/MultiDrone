#ifndef KEY_PAIR_H
#define KEY_PAIR_H

#include "Point.h"
#include "EllipticCurve.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

class KeyPair {

private:
    cpp_int privateKey;
    cpp_int publicKey;
    Point publicKey;
public:
    KeyPair(const EllipticCurve& curve);
    cpp_int getPrivateKey() const;
    Point getPublicKey() const;
    cpp_int generate_random_cpp_int(int bit_length);
    cpp_int generate_prime(int bit_length);
    cpp_int gcd(cpp_int a, cpp_int b);
    void set_private_and_public_keys();

}; 

#endif // KEY_PAIR_H
