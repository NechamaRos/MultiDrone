#include "doctest.h"
#include "HuffmanRescue.h"
#include "HuffmanCompression.h"
TEST_CASE("Huffman Rescue Tests") {

    SUBCASE("Test full compression and rescue cycle") {
        // Test the entire process of compression followed by rescue
        array<size_t, 2> dims2D = { 50, 50 };
        Image<2> originalImg = createRandomImage(dims2D);
        Node* root = buildHuffmanTree(originalImg);
        map<unsigned char, string> huffmanCode;
        generateCodes(root, "", huffmanCode);
        string compressed = compressImage(originalImg).getCompressedData();
        Image<2> decompressedImg = decompressImage<2>(compressed, huffmanCode, dims2D);

        // Check if the decompressed image matches the original
        CHECK(decompressedImg.dimensions == originalImg.dimensions);
        CHECK(decompressedImg.pixels.size() == originalImg.pixels.size());
        CHECK(decompressedImg.pixels == originalImg.pixels);
        delete root;
    }

    SUBCASE("Test rescue with single pixel value image") {
        // Test rescue with an image containing only one pixel value
        array<size_t, 2> dims2D = { 10, 10 };
        Image<2> originalImg = createSimpleImage(dims2D, static_cast<unsigned char>(123));
        Node* root = buildHuffmanTree(originalImg);
        map<unsigned char, string> huffmanCode;
        generateCodes(root, "", huffmanCode);
        string compressed = compressImage(originalImg).getCompressedData();
        Image<2> decompressedImg = decompressImage<2>(compressed, huffmanCode, dims2D);

        CHECK(decompressedImg.dimensions == originalImg.dimensions);
        CHECK(all_of(decompressedImg.pixels.begin(), decompressedImg.pixels.end(),
            [](unsigned char pixel) { return pixel == 123; }));
        delete root;
    }

    SUBCASE("Test rescue with large image") {
        // Test rescue with a large image
        array<size_t, 2> dims2D = { 1000, 1000 };
        Image<2> originalImg = createRandomImage(dims2D);
        Node* root = buildHuffmanTree(originalImg);
        map<unsigned char, string> huffmanCode;
        generateCodes(root, "", huffmanCode);
        string compressed = compressImage(originalImg).getCompressedData();
        Image<2> decompressedImg = decompressImage<2>(compressed, huffmanCode, dims2D);

        CHECK(decompressedImg.dimensions == originalImg.dimensions);
        // Check a sample of pixels to ensure correct rescue
        for (size_t i = 0; i < 100; ++i) {
            size_t index = rand() % originalImg.pixels.size();
            CHECK(decompressedImg.pixels[index] == originalImg.pixels[index]);
        }
        delete root;
    }
}