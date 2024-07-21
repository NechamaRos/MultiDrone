#include "KeyPair.h"
#include "Utils.h"
#include "Logger.h"

KeyPair::KeyPair(const EllipticCurve& curve) {

    privateKey = generateRandomPrivateKey(curve.getN());

    try {
        publicKey = curve.multiply(curve.getG(), privateKey);
    }
    catch (const std::runtime_error& e) {
        Logger::getInstance().log(e.what());
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

cpp_int KeyPair::getPrivateKey() const {
    return privateKey;
}

Point KeyPair::getPublicKey() const {
    return publicKey;
}
