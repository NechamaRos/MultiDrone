#ifndef ECC_IMPL_H
#define ECC_IMPL_H

#include "ECC.h"
#include "Logger.h"
#include "Utils.h"
#include <CL/sycl.hpp>
#include <vector>
#include <stdexcept>

using namespace std;
using namespace sycl;

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
    Logger::getInstance().log("encrypt: the point K is " + k.str());

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

    // Fill the remaining bytes with zeros if x_bytes is smaller than sizeof(T)
    while (x_bytes.size() < sizeof(T)) {
        x_bytes.push_back(0);
    }

    // Encrypt the data using XOR with the bytes of x
    vector<unsigned char> ciphertext(sizeof(T));
    const unsigned char* dataBytes = reinterpret_cast<const unsigned char*>(data);

    // SYCL queue
    queue q;

    // Buffers for dataBytes and ciphertext
    buffer<unsigned char, 1> dataBytesBuf(dataBytes, range<1>(sizeof(T)));
    buffer<unsigned char, 1> ciphertextBuf(ciphertext.data(), range<1>(sizeof(T)));
    buffer<unsigned char, 1> xBytesBuf(x_bytes.data(), range<1>(x_bytes.size()));

    q.submit([&](handler& h) {
        auto dataAcc = dataBytesBuf.get_access<access::mode::read>(h);
    auto cipherAcc = ciphertextBuf.get_access<access::mode::write>(h);
    auto xBytesAcc = xBytesBuf.get_access<access::mode::read>(h);

    h.parallel_for(range<1>(sizeof(T)), [=](id<1> i) {
        cipherAcc[i] = dataAcc[i] ^ xBytesAcc[i % xBytesAcc.get_count()];
        });
        }).wait();

        // Copy the results back to the ciphertext vector
        auto ciphertextAcc = ciphertextBuf.get_access<access::mode::read>();
        for (size_t i = 0; i < ciphertext.size(); ++i) {
            ciphertext[i] = ciphertextAcc[i];
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

    cpp_int x_value = S.getX();

    // Convert x_value to bytes
    vector<unsigned char> x_bytes;
    while (x_value > 0) {
        x_bytes.push_back(static_cast<unsigned char>(x_value % 256));
        x_value >>= 8;
    }

    // Fill the remaining bytes with zeros if x_bytes is smaller than sizeof(T)
    while (x_bytes.size() < sizeof(T)) {
        x_bytes.push_back(0);
    }

    // Decrypt the data using XOR with the bytes of x
    T* decryptedData = new T;
    unsigned char* decryptedDataBytes = reinterpret_cast<unsigned char*>(decryptedData);

    // SYCL queue
    queue q;

    // Buffers for encryptedMessage.ciphertext and decryptedDataBytes
    buffer<unsigned char, 1> cipherBuf(encryptedMessage.ciphertext.data(), range<1>(sizeof(T)));
    buffer<unsigned char, 1> decryptedBuf(decryptedDataBytes, range<1>(sizeof(T)));
    buffer<unsigned char, 1> xBytesBuf(x_bytes.data(), range<1>(x_bytes.size()));

    q.submit([&](handler& h) {
        auto cipherAcc = cipherBuf.get_access<access::mode::read>(h);
    auto decryptedAcc = decryptedBuf.get_access<access::mode::write>(h);
    auto xBytesAcc = xBytesBuf.get_access<access::mode::read>(h);

    h.parallel_for(range<1>(sizeof(T)), [=](id<1> i) {
        decryptedAcc[i] = cipherAcc[i] ^ xBytesAcc[i % xBytesAcc.get_count()];
        });
        }).wait();

        // Copy the results back to the decrypted data
        auto decryptedAcc = decryptedBuf.get_access<access::mode::read>();
        for (size_t i = 0; i < sizeof(T); ++i) {
            decryptedDataBytes[i] = decryptedAcc[i];
        }

        return decryptedData;
}

#endif // ECC_IMPL_H
