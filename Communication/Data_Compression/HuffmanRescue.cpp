#include "HuffmanRescue.h"

map<string, unsigned char> createReverseHuffmanCode(const map<unsigned char, string>& huffmanCode) {
    map<string, unsigned char> reverseCode;
    for (const auto& pair : huffmanCode) {
        reverseCode[pair.second] = pair.first;
    }
    return reverseCode;
}

vector<char> decompressImage(const Data_Compression<1>& compress) {
    return convertImageToSimpleMessage(decompressImage(compress.getCompressedData(), compress.getHuffmanCode(), compress.getDimension()));
}

vector<vector<uint8_t>> decompressImage(const Data_Compression<2>& compress) {
    return convertImageToBlackWhite(decompressImage(compress.getCompressedData(), compress.getHuffmanCode(), compress.getDimension()));
}

vector<vector<vector<uint8_t>>> decompressImage(const Data_Compression<3>& compress) {
    return convertImageToColor(decompressImage(compress.getCompressedData(), compress.getHuffmanCode(), compress.getDimension()));
}
