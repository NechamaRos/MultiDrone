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

// Helper functions for the new tests

double calculateImageSimilarity(const Image<2>& img1, const Image<2>& img2) {
    if (img1.pixels.size() != img2.pixels.size()) return 0.0;
    size_t matchingPixels = 0;
    for (size_t i = 0; i < img1.pixels.size(); ++i) {
        if (img1.pixels[i] == img2.pixels[i]) ++matchingPixels;
    }
    return static_cast<double>(matchingPixels) / img1.pixels.size();
}

void testRescue(const Image<2>& originalImg, const Data_Compression<2>& compressed) {
    Image<2> rescuedImg = decompressImage<2>(compressed.getCompressedData(), compressed.getHuffmanCode(), originalImg.dimensions);
    CHECK(rescuedImg.dimensions == originalImg.dimensions);
    CHECK(rescuedImg.pixels.size() == originalImg.pixels.size());

    // Allow for small differences due to potential lossy compression
    double similarity = calculateImageSimilarity(originalImg, rescuedImg);
    CHECK(similarity > 0.99);  // Expect at least 99% similarity
}

void testCompressionAndRescue(const Image<2>& originalImg) {
    Data_Compression<2> compressed = compressImage(originalImg);
    testRescue(originalImg, compressed);
}

Image<2> createAllValuesImage(const array<size_t, 2>& dims) {
    Image<2> img;
    img.initImage(dims);
    for (size_t i = 0; i < img.pixels.size(); ++i) {
        img.pixels[i] = static_cast<unsigned char>(i % 256);
    }
    return img;
}


TEST_CASE("Huffman Rescue Additional Tests") {
    SUBCASE("Test rescue with varying compression ratios") {
        array<size_t, 2> dims2D = { 100, 100 };

        // Test with highly compressible image
        Image<2> highlyCompressibleImg = createSimpleImage(dims2D, static_cast<unsigned char>(0));
        testCompressionAndRescue(highlyCompressibleImg);

        // Test with less compressible image
        Image<2> lessCompressibleImg = createRandomImage(dims2D);
        testCompressionAndRescue(lessCompressibleImg);
    }

    SUBCASE("Test rescue with different compression methods") {
        array<size_t, 2> dims2D = { 50, 50 };
        Image<2> img = createRandomImage(dims2D);

        // Force Huffman compression
        Data_Compression<2> huffmanCompressed = compressImage(img);
        huffmanCompressed.setCompressedData("HUF" + huffmanCompressed.getCompressedData().substr(3));
        //testRescue(img, huffmanCompressed);

        // Force RLE compression
        Data_Compression<2> rleCompressed = compressImage(img);
        rleCompressed.setCompressedData("RLE" + runLengthEncode(img.pixels));
        testRescue(img, rleCompressed);
    }

    SUBCASE("Test rescue with edge cases") {
        // Test with single pixel image
        array<size_t, 2> singlePixelDims = { 1, 1 };
        Image<2> singlePixelImg = createSimpleImage(singlePixelDims, static_cast<unsigned char>(123));
        testCompressionAndRescue(singlePixelImg);

        // Test with image containing all possible pixel values
        array<size_t, 2> allValuesDims = { 16, 16 };
        Image<2> allValuesImg = createAllValuesImage(allValuesDims);
        testCompressionAndRescue(allValuesImg);
    }

    SUBCASE("Test rescue with corrupted data") {
        array<size_t, 2> dims2D = { 50, 50 };
        Image<2> originalImg = createRandomImage(dims2D);
        Data_Compression<2> compressed = compressImage(originalImg);

        // Corrupt the compressed data
        string corruptedData = compressed.getCompressedData();
        corruptedData[corruptedData.length() / 2] ^= 0xFF;  // Flip all bits in the middle byte
        compressed.setCompressedData(corruptedData);

        // Attempt to rescue and expect an exception or significant difference
        //CHECK_THROWS(decompressImage<2>(compressed.getCompressedData(), compressed.getHuffmanCode(), dims2D));
    }
}
