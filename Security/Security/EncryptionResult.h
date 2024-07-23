#ifndef ENCRYPTIONRESULT_H
#define ENCRYPTIONRESULT_H

#include <vector>
#include "Point.h" // אם אתה צריך להגדיר את סוג ה-Point

struct EncryptionResult {
    std::vector<unsigned char> ciphertext;
    Point R; // ודא שה-Point מוגדר כאן או כלול בקובץ זה
};

#endif // ENCRYPTIONRESULT_H
