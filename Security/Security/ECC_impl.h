#ifndef ECC_IMPL_H
#define ECC_IMPL_H

#include "ECC.h"
#include "Logger.h"
#include "Utils.h"
using namespace std;

// Function to encrypt data using ECC
template<typename T>
EncryptionResult ECC::encrypt(const T* data, const Point& publicKey) const {
    // Check if the data is not null
    if (data == nullptr) {
        Logger::getInstance().log("encrypt: the data received is null");
        throw std::runtime_error("The data received is null");
    }
    // Check if the public key is on the curve
    if (!curve.isOnCurve(publicKey)) {
        Logger::getInstance().log("encrypt: The public key is not correct - it is not on the curve");
        throw std::runtime_error("The public key is not correct - it is not on the curve");
    }

    // Generate a random number k
    cpp_int k = generateSecureRandomNumber(1, curve.getN());
    // Compute the points R and S
    Point R = curve.multiply(curve.getG(), k);
    Point S = curve.multiply(publicKey, k);

    // Convert data to a vector of unsigned char
    vector<unsigned char> plaintext(data, data + sizeof(T));
    vector<unsigned char> ciphertext;

    // Convert the x value of S to bytes
    cpp_int x_value = S.getX();
    vector<unsigned char> x_bytes;
    while (x_value > 0) {
        x_bytes.push_back(static_cast<unsigned char>(x_value % 256));
        x_value >>= 8;
    }

    // Check if x_bytes is not empty
    if (x_bytes.empty()) {
        Logger::getInstance().log("encrypt: x_bytes is empty");
        throw std::runtime_error("x_bytes is empty");
    }

    // Encrypt the data using XOR with the bytes of x
    for (unsigned char c : plaintext) {
        unsigned char encrypted_char = c ^ (x_bytes[0]); // Use the first byte of S.getX()
        ciphertext.push_back(encrypted_char);
    }
    return { ciphertext, R };
}

// Function to decrypt data using ECC
template<typename T>
T* ECC::decrypt(const EncryptionResult& encryptedMessage, cpp_int privateKey) const {
    // Check if the encrypted message and the point R are valid
    if (encryptedMessage.ciphertext.empty() || encryptedMessage.R.isInfinity()) {
        Logger::getInstance().log("decrypt: The encryptedMessage has an invalid value");
        throw std::runtime_error("The encryptedMessage has an invalid value");
    }
    // Compute the point S by multiplying R with the private key
    Point S = curve.multiply(encryptedMessage.R, privateKey);

    // Decrypt the ciphertext using XOR with the bytes of x
    vector<unsigned char> ciphertext = encryptedMessage.ciphertext;
    vector<unsigned char> plaintext;

    // Convert the x value of S to bytes
    cpp_int x_value = S.getX();
    vector<unsigned char> x_bytes;
    while (x_value > 0) {
        x_bytes.push_back(static_cast<unsigned char>(x_value % 256));
        x_value >>= 8;
    }

    // Check if x_bytes is not empty
    if (x_bytes.empty()) {
        Logger::getInstance().log("decrypt: x_bytes is empty");
        throw std::runtime_error("x_bytes is empty");
    }

    // Decrypt the data using XOR with the bytes of x
    for (unsigned char c : ciphertext) {
        unsigned char decrypted_char = c ^ (x_bytes[0]); // Use the first byte of S.getX()
        plaintext.push_back(decrypted_char);
    }

    // Create a decrypted message from the plaintext data
    T* decryptedMessage = new T;
    copy(plaintext.begin(), plaintext.begin() + sizeof(T), reinterpret_cast<unsigned char*>(decryptedMessage));

    return decryptedMessage;
}

#endif // ECC_IMPL_H
