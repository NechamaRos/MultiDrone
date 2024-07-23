#include "KeyPair.h"
#include "Utils.h"
#include "Logger.h"

KeyPair::KeyPair(const EllipticCurve& curve) {

    privateKey = generateRandomPrivateKey(curve.getN());

    try {
        publicKey = curve.multiply(curve.getG(), 10);
        Logger::getInstance().log("KeyPair :pk.getX(): " + to_string(publicKey.getX()));
    }
    catch (const std::runtime_error& e) {
        Logger::getInstance().log(e.what());
        throw std::runtime_error( e.what() );
        
    }
}

cpp_int KeyPair::getPrivateKey() const {
    return privateKey;
}

Point KeyPair::getPublicKey() const {
    return publicKey;
}
