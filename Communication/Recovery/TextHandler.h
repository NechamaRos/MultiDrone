#pragma once
#include "DataHandler.h"
#include <cstdint>
#include<vector>
#include<string>
using namespace std;
class TextHandler {
private:
    //string messege;
public:
    TextHandler();
    vector<uint8_t> toOneDementionalArray(string text);
   /* TextHandler(string messege);
    Poly toPolynomial(GaloisField& gf) override;
    void fromPolynomial(Poly& poly) override;*/
};