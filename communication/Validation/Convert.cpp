#include "Convert.h"

vector<uint8_t> Convert::convertCharToUint8(char c)
{
    vector<uint8_t> res;

    while (c) {
        if (c & 1)
            res.push_back(1);
        else
            res.push_back(0);
        c = c >> 1;
    }
    for (int i = res.size(); i < 8; i++)
        res.push_back(0);
    reverse(res.begin(), res.end());
    return res;
}

vector<uint8_t> Convert::convertCharVecToUint8(vector<char> c)
{
     vector<uint8_t> res,one;
     int len = c.size();
     for (int i = 0; i < len; i++) {
         one = convertCharToUint8(c[i]);
         for (int j = 0; j < 8; j++) {
             res.push_back(one[j]);
         }
     }
     return res;
}

vector<char> Convert::convertUint8ToChar(vector<uint8_t> c)
{
    char ch = 0;
    vector<char> res;
    for (auto i = c.begin(); i < c.end();) {
        ch = 0;
        for (int j = 0; j < 8 && i < c.end(); j++, i++) {
            ch = ch << 1;
            if (*i)
                ch = ch | 1;
        }
        res.push_back(ch);
    }
    return res;

}

