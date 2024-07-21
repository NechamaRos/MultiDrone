#include "Utils.h"
#include <cstdlib>
#include <algorithm> // הכנסה של הקובץ המתאים
#include <random>


cpp_int mod(cpp_int a, cpp_int b) {
    if (b < 0) b = -b;
    cpp_int result = a % b;
    if (result < 0) result += b;
    return result;
}

cpp_int modInverse(cpp_int a, cpp_int m) {
    cpp_int m0 = m, t, q;
    cpp_int x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

cpp_int gcd(cpp_int a, cpp_int b) {
    while (b != 0) {
        cpp_int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

cpp_int generateRandomPrivateKey(cpp_int n) {
    return rand() % (n - 1) + 1;
}
