#ifndef ECC_H
#define ECC_H

#include "EllipticCurve.h"
#include "EncryptionResult.h"
#include "Point.h"
#include <vector>
#include "Utils.h"
using namespace std;


class ECC {
private:

    EllipticCurve curve;
  

public:

    ECC(const EllipticCurve& curve);
    template <typename T>
    EncryptionResult encrypt(const T* data, size_t dataSize, const Point& publicKey) const;
    /*Point decrypt(const Point& encryptedMessage, int privateKey) const;
    Point sign(const Point& message, int privateKey) const;
    bool verify(const Point& message, const Point& signature, const Point& publicKey) const;*/
};

#endif // ECC_H
 
template<typename T>
inline EncryptionResult ECC::encrypt(const T* data, size_t dataSize, const Point& publicKey) const {
    if (data == nullptr || dataSize <= 0) {
        Logger::getInstance().log("encrypt: the data received is null");
        throw std::runtime_error("The data received is null");
    }
    if (!curve.isOnCurve(publicKey)) {
        Logger::getInstance().log("encrypt: The public key is not correct - it is not on the curve");
        throw std::runtime_error("The public key is not correct - it is not on the curve");
    }

    cpp_int k = generateSecureRandomNumber(1, curve.getN());
    Point R;
    Point S;
    try {
        R = curve.multiply(curve.getG(), k);
        S = curve.multiply(publicKey, k);
    }
    catch (const std::runtime_error& e) {
        Logger::getInstance().log("encrypt: " + std::string(e.what()));
        throw std::runtime_error(e.what());
    }

    // Convert data to a vector of unsigned char
    vector<unsigned char> plaintext(data, data + dataSize);
    vector<unsigned char> ciphertext;

    cpp_int x_value = S.getX();
    if (x_value <= 0) {
        Logger::getInstance().log("encrypt: S.getX() is non-positive");
        throw std::runtime_error("S.getX() is non-positive");
    }

    std::vector<unsigned char> x_bytes;
    while (x_value > 0) {
        x_bytes.push_back(static_cast<unsigned char>(x_value % 256));
        x_value >>= 8;
    }

    // Ensure x_bytes is not empty
    if (x_bytes.empty()) {
        Logger::getInstance().log("encrypt: x_bytes is empty");
        throw std::runtime_error("x_bytes is empty");
    }

    for (unsigned char c : plaintext) {
        unsigned char encrypted_char = c ^ (x_bytes[0]); // Use the first byte of S.getX()
        ciphertext.push_back(encrypted_char);
    }
    Logger::getInstance().log(ciphertextToHexString(ciphertext));
    return { ciphertext, R };
}

