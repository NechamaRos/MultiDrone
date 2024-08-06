#ifndef ECC_IMPL_H
#define ECC_IMPL_H

#include "ECC.h"
#include "Logger.h"
#include "Utils.h"
#define CHAR_BIT_ON 0xff;
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
    Logger::getInstance().log("encrypt: the point K is "+k.str());
    //Logger::getInstance().log(k.str());


    // Compute the points R and S
    Point R = curve.multiply(curve.getG(), k);
    Point S = curve.multiply(publicKey, k);

    // Convert the x value of S to a vector of bytes
    cpp_int x_value = S.getX();
    vector<unsigned char> x_bytes;
    while (x_value > 0) {
        x_bytes.push_back(static_cast<unsigned char>(x_value % 256));
        x_value >>= 8;
    }

    // Encrypt the data using XOR with the bytes of x
    vector<unsigned char> ciphertext(sizeof(T));
    const unsigned char* dataBytes = reinterpret_cast<const unsigned char*>(data);
    for (size_t i = 0; i < sizeof(T); ++i) {
        ciphertext[i] = dataBytes[i] ^ x_bytes[i % x_bytes.size()];
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

    // Convert the x value of S to a vector of bytes
    cpp_int x_value = S.getX();
   /* vector<unsigned char> x_bytes;
    while (x_value > 0) {
        x_bytes.push_back(static_cast<unsigned char>(x_value % 256));
        x_value >>= 8;
    }*/
    


    // Decrypt the data using XOR with the bytes of x
    T* decryptedData = new T;
     
    unsigned char* decryptedDataBytes = reinterpret_cast<unsigned char*>(decryptedData);
    for (size_t i = 0; i < sizeof(T); ++i) {
        char mask = static_cast<unsigned char>(x_value) & CHAR_BIT_ON;
        decryptedDataBytes[i] = encryptedMessage.ciphertext[i] ^ mask;
        x_value >>= 8;
    }

    return decryptedData;
}

#endif // ECC_IMPL_H
