#include "functions.h"
using namespace std;
struct BMP {
    int width;
    int height;
    vector<unsigned char> pixels;
};

Image readImage(string filename) {
    BMP image;

    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file");
    }

    file.seekg(18, ios::beg);
    file.read(reinterpret_cast<char*>(&image.width), sizeof(image.width));
    file.read(reinterpret_cast<char*>(&image.height), sizeof(image.height));

    file.seekg(54, ios::beg); // Skip to pixel data

    int rowSize = ((image.width * 3 + 3) / 4) * 4; // Calculate padded row size
    image.pixels.resize(rowSize * image.height);

    for (int i = 0; i < image.height; i++) {
        file.read(reinterpret_cast<char*>(&image.pixels[(image.height - 1 - i) * rowSize]), rowSize);
    }

    file.close();

    Image result(image.width, image.height, 3, image.pixels);
    int p;
    return result;
}