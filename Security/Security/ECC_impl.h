#ifndef ECC_IMPL_H
#define ECC_IMPL_H

#include "ECC.h"
#include "Logger.h"
#include "Utils.h"
using namespace std;

template<typename T>
EncryptionResult ECC::encrypt(const T* data, const Point& publicKey) const {
    if (data == nullptr ) {
        Logger::getInstance().log("encrypt: the data received is null");
        throw std::runtime_error("The data received is null");
    }
    if (!curve.isOnCurve(publicKey)) {
        Logger::getInstance().log("encrypt: The public key is not correct - it is not on the curve");
        throw std::runtime_error("The public key is not correct - it is not on the curve");
    }

    cpp_int k = generateSecureRandomNumber(1, curve.getN());
    Point R = curve.multiply(curve.getG(), k);
    Point S = curve.multiply(publicKey, k);

    // Convert data to a vector of unsigned char
    vector<unsigned char> plaintext(data, data + sizeof(T));
    vector<unsigned char> ciphertext;

    cpp_int x_value = S.getX();
    vector<unsigned char> x_bytes;
    while (x_value > 0) {
        x_bytes.push_back(static_cast<unsigned char>(x_value % 256));
        x_value >>= 8;
    }

    if (x_bytes.empty()) {
        Logger::getInstance().log("encrypt: x_bytes is empty");
        throw std::runtime_error("x_bytes is empty");
    }

    for (unsigned char c : plaintext) {
        unsigned char encrypted_char = c ^ (x_bytes[0]); // Use the first byte of S.getX()
        ciphertext.push_back(encrypted_char);
    }
    return { ciphertext, R };
}

template<typename T>
T* ECC::decrypt(const EncryptionResult& encryptedMessage, cpp_int privateKey) const {
    if (encryptedMessage.ciphertext.empty() || encryptedMessage.R.isInfinity()) {
        Logger::getInstance().log("decrypt :The encryptedMessage with an invalid value");
        throw std::runtime_error("The encryptedMessage with an invalid value");
    }
    Point S = curve.multiply(encryptedMessage.R, privateKey);

    vector<unsigned char> ciphertext = encryptedMessage.ciphertext;
    vector<unsigned char> plaintext;

    cpp_int x_value = S.getX();
    vector<unsigned char> x_bytes;
    while (x_value > 0) {
        x_bytes.push_back(static_cast<unsigned char>(x_value % 256));
        x_value >>= 8;
    }

    if (x_bytes.empty()) {
        Logger::getInstance().log("decrypt: x_bytes is empty");
        throw std::runtime_error("x_bytes is empty");
    }

    for (unsigned char c : ciphertext) {
        unsigned char decrypted_char = c ^ (x_bytes[0]); // Use the first byte of S.getX()
        plaintext.push_back(decrypted_char);
    }

    T* decryptedMessage = new T;
    copy(plaintext.begin(), plaintext.begin() + sizeof(T), reinterpret_cast<unsigned char*>(decryptedMessage));

    return decryptedMessage;
}

#endif // ECC_IMPL_H
