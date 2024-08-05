#pragma once
#include "DataHandler.h"
#include <cstdint>
#include <vector>
using namespace std;
class ColorImageHandler 
{
private:
    //vector<vector<vector<uint8_t>>> imageData;
public:
    vector<uint8_t> toOneDementionalArray(vector<vector<vector<uint8_t>>>& imageData);
    //ColorImageHandler(vector<vector<vector<uint8_t>>>& inputImage);
    //Poly toPolynomial(GaloisField& gf) override;   
    //void fromPolynomial(Poly& poly) override;
    //vector<vector<vector<uint8_t>>> getImageData();
};

