#ifndef KEY_PAIR_H
#define KEY_PAIR_H

#include "Point.h"
#include "EllipticCurve.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

class KeyPair {

private:
    cpp_int privateKey;
    Point publicKey;
public:
    KeyPair(const EllipticCurve& curve);
    cpp_int getPrivateKey() const;
    Point getPublicKey() const;
};

#endif // KEY_PAIR_H
