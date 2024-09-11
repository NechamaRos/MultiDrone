#ifndef KEY_PAIR_H
#define KEY_PAIR_H

#include "Point.h"
#include "EllipticCurve.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

class KeyPair {

private:
    cpp_int privateKey;
    cpp_int publicKey_RSA;
    Point publicKey;

public:
    KeyPair(const EllipticCurve& curve);

    cpp_int getPrivateKey() const;
    cpp_int getPublicKey_RSA() const;
    Point getPublicKey() const;

    void setPrivateKey(cpp_int privateKey);
    void setPublicKey_RSA(cpp_int publicKey_RSA);
    void setPublicKey(Point publicKey);
};

#endif // KEY_PAIR_H
