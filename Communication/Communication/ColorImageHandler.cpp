#include "ColorImageHandler.h"

vector<uint8_t> ColorImageHandler::toOneDementionalArray(vector<vector<vector<uint8_t>>>& imageData)
{
    vector<uint8_t> flattendData;
    for (int i = 0; i < imageData.size(); i++) {
        for (int j = 0; j < imageData[i].size(); j++) {
            for(int k=0;k< 3; k++)
                flattendData.push_back(imageData[i][j][k]);
        }
    }
    return flattendData;
}


