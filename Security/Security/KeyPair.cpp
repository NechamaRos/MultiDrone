#include "KeyPair.h"
#include "Utils.h"
#include "Logger.h"

// Constructor for the KeyPair class
KeyPair::KeyPair(const EllipticCurve& curve) {

    // Generate a random private key within the range [1, N-1]
    privateKey = generateRandomPrivateKey(curve.getN());

    try {
        // Calculate the public key by multiplying the generator point G with the private key
        publicKey = curve.multiply(curve.getG(), privateKey);
        //Logger::getInstance().log("KeyPair :pk.getX(): " + to_string(publicKey.getX()));
    }
    catch (const std::runtime_error& e) {
        // Log the error message and rethrow the exception
        Logger::getInstance().log(e.what());
        throw std::runtime_error(e.what());
    }
}
// Getter for the private key
cpp_int KeyPair::getPrivateKey() const {
    return privateKey;
}

cpp_int KeyPair::getPublicKey_RSA() const {
    return publicKey_RSA;
}

// Getter for the public key
Point KeyPair::getPublicKey() const {
    return publicKey;
}

void KeyPair::setPrivateKey(cpp_int privateKey) {
    this->privateKey = privateKey;
}

void KeyPair::setPublicKey_RSA(cpp_int publicKey_RSA) {
    this->publicKey_RSA = publicKey_RSA;
}

void KeyPair::setPublicKey(Point publicKey) {
    this->publicKey = publicKey;
}