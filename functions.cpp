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

void writeImage(const std::string& filename, const Image& image) {
    std::ofstream bmpFile(filename, std::ios::out | std::ios::binary);
    if (!bmpFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    int rowSize = image.width * 3; // Row size including padding
    int padding = (4 - (rowSize % 4)) % 4;
    int rowSizeWithPadding = rowSize + padding;
    int fileSize = 54 + rowSizeWithPadding * image.height; // Total file size
    int imageSize = rowSizeWithPadding * image.height;


    bmpFile.put('B').put('M'); // BMP magic number
    bmpFile.write(reinterpret_cast<const char*>(&fileSize), 4); // File size
    bmpFile.write("\0\0\0\0", 4); // Reserved
    int offsetToData = 54;
    bmpFile.write(reinterpret_cast<const char*>(&offsetToData), 4); // Offset to image data

    // DIB Header (BITMAPINFOHEADER)
    int headerSize = 40;
    bmpFile.write(reinterpret_cast<const char*>(&headerSize), 4); // DIB header size
    bmpFile.write(reinterpret_cast<const char*>(&image.width), 4); // Image width
    bmpFile.write(reinterpret_cast<const char*>(&image.height), 4); // Image height
    short planes = 1;
    bmpFile.write(reinterpret_cast<const char*>(&planes), 2); // Planes
    short bitsPerPixel = 24;
    bmpFile.write(reinterpret_cast<const char*>(&bitsPerPixel), 2); // Bits per pixel
    int compression = 0;
    bmpFile.write(reinterpret_cast<const char*>(&compression), 4); // Compression method
    bmpFile.write(reinterpret_cast<const char*>(&imageSize), 4); // Image size
    int resolutionX = 2835; // Pixels per meter (72 DPI)
    int resolutionY = 2835; // Pixels per meter (72 DPI)
    bmpFile.write(reinterpret_cast<const char*>(&resolutionX), 4); // Horizontal resolution
    bmpFile.write(reinterpret_cast<const char*>(&resolutionY), 4); // Vertical resolution
    int colorsUsed = 0;
    bmpFile.write(reinterpret_cast<const char*>(&colorsUsed), 4); // Colors in color table
    int importantColors = 0;
    bmpFile.write(reinterpret_cast<const char*>(&importantColors), 4); // Important color count

    // Write image data (bottom to top)
    for (int y = image.height-1; y >=0; y--) {
        for (int x = 0; x < image.width; x++) {
            int index = (y * image.width + x) * 3+padding*y;
            bmpFile.write(reinterpret_cast<const char*>(&image.data[index]), 1); // Blue channel
            bmpFile.write(reinterpret_cast<const char*>(&image.data[index + 1]), 1); // Green channel
            bmpFile.write(reinterpret_cast<const char*>(&image.data[index+2]), 1); // Red channel
        }
        // Write padding bytes
        for (int p = 0; p < padding; p++) {
            bmpFile.write("\0", 1);
        }
    }

    bmpFile.close();
}

void DisplayImage(const Image& img) {
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = img.width;
    bmi.bmiHeader.biHeight = -img.height;  // Minus height to display image right-side up
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;  // Assuming 24-bit RGB image
    bmi.bmiHeader.biCompression = BI_RGB;
    //HWND console = GetConsoleWindow();
    //PAINTSTRUCT ps;
    //HDC hdc = GetDC(console);

   HDC hdc = GetDC(NULL);
    StretchDIBits(hdc, 0, 0, img.width, img.height, 0, 0, img.width, img.height, img.data, &bmi, DIB_RGB_COLORS, SRCCOPY);
    //Sleep(30000);  // Sleep for 30,000 milliseconds (30 seconds)

   ReleaseDC(NULL, hdc);
   
    //EndPaint(console, &ps);
}
