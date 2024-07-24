#ifndef ECC_H
#define ECC_H

#include "EllipticCurve.h"
#include "EncryptionResult.h"
#include "Point.h"
#include <vector>
#include "Utils.h"
#include <type_traits>
using namespace std;

class ECC {
private:
    EllipticCurve curve;

public:
    ECC(const EllipticCurve& curve) : curve(curve) {}  // Initialization list

    template <typename T>
    EncryptionResult encrypt(const T* data, const Point& publicKey) const;

    template <typename T>
    T* decrypt(const EncryptionResult& encryptedMessage, cpp_int privateKey) const;
};

#include "ECC_impl.h"  // Ensure this is included only once.

#endif // ECC_H
