#ifndef ENCRYPTIONRESULT_H
#define ENCRYPTIONRESULT_H

#include <vector>
#include "Point.h" 

struct EncryptionResult {
    std::vector<unsigned char> ciphertext;
    Point R;
};
//
//struct DecryptionResult {
//    std::vector<unsigned char> ciphertext;
//    Point R;
//};

#endif // ENCRYPTIONRESULT_H
 