#include "BWImageHandler.h"

vector<uint8_t> BWImageHandler::toOneDementionalArray(vector<vector<uint8_t>>& imageData)
{
    vector<uint8_t> flattendData;
    for (int i = 0; i < imageData.size(); i++) {
        for (int j = 0; j < imageData[i].size(); j++) {
            flattendData.push_back(imageData[i][j]);
        }
    }
    return flattendData;
}


