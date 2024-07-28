#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "HuffmanCompression.h"
#include "HuffmanRescue.h"
using namespace std;

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);

    // Compression for 2D image
    array<size_t, 2> dims2D = { 64, 64 };
    Image<2> sampleImg2D = createSampleImage(dims2D);

    Node* root2D = buildHuffmanTree(sampleImg2D);

    map<unsigned char, string> huffmanCode2D;
    generateCodes(root2D, "", huffmanCode2D);

    string compressedImg2D = compressImage(sampleImg2D, huffmanCode2D);

    double compressionRatio2D = static_cast<double>(dims2D[0] * dims2D[1] * 8) / compressedImg2D.length();
    cout << "Compression ratio for 2D image: " << compressionRatio2D << endl;

    delete root2D;

    vector<unsigned char> compressedData2D = bitStringToBytes(compressedImg2D);
    auto reverseHuffmanCode2D = createReverseHuffmanCode(huffmanCode2D);
    string bitString2D = bytesToBitString(compressedData2D);
    Image<2> decompressedImage2D = createRescueImage(bitString2D, reverseHuffmanCode2D, dims2D);

    if (compareImages(sampleImg2D, decompressedImage2D)) {
        cout << "The decompressed 2D image is identical to the original image." << endl;
    }
    else {
        cout << "The decompressed 2D image is NOT identical to the original image." << endl;
    }

    // Compression for 3D image
    array<size_t, 3> dims3D = { 32, 32, 32 };
    Image<3> sampleImg3D = createSampleImage(dims3D);

    Node* root3D = buildHuffmanTree(sampleImg3D);

    map<unsigned char, string> huffmanCode3D;
    generateCodes(root3D, "", huffmanCode3D);

    string compressedImg3D = compressImage(sampleImg3D, huffmanCode3D);

    double compressionRatio3D = static_cast<double>(dims3D[0] * dims3D[1] * dims3D[2] * 8) / compressedImg3D.length();
    cout << "Compression ratio for 3D image: " << compressionRatio3D << endl;

    delete root3D;

    vector<unsigned char> compressedData3D = bitStringToBytes(compressedImg3D);
    auto reverseHuffmanCode3D = createReverseHuffmanCode(huffmanCode3D);
    string bitString3D = bytesToBitString(compressedData3D);
    Image<3> decompressedImage3D = createRescueImage(bitString3D, reverseHuffmanCode3D, dims3D);

    if (compareImages(sampleImg3D, decompressedImage3D)) {
        cout << "The decompressed 3D image is identical to the original image." << endl;
    }
    else {
        cout << "The decompressed 3D image is NOT identical to the original image." << endl;
    }

    int res = context.run();

    if (context.shouldExit()) {
        return res;
    }

    return res;
}
