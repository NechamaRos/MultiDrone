#include "doctest.h"
#include "HuffmanRescue.h"
#include "HuffmanCompression.h"


TEST_CASE("Test full compression and rescue cycle") {
	// Test the entire process of compression followed by rescue
	array<size_t, 2> dims2D = { 50,50 };
	Image<2> originalImg = createRandomImage(dims2D);
	Node* root = buildHuffmanTree(originalImg);
	map<unsigned char, string> huffmanCode;
	generateCodes(root, "", huffmanCode);

	string compressed = compressImage(originalImg, huffmanCode);

	vector<unsigned char> compressedData = bitStringToBytes(compressed);
	auto reverseHuffmanCode = createReverseHuffmanCode(huffmanCode);
	string bitString = bytesToBitString(compressedData);
	Image<2> decompressedImg = createRescueImage(bitString, reverseHuffmanCode, dims2D);

	// Check if the decompressed image matches the original
	CHECK(decompressedImg.dimensions[0] == originalImg.dimensions[0]);
	CHECK(decompressedImg.dimensions[1] == originalImg.dimensions[1]);
	for (int y = 0; y < originalImg.dimensions[0] * originalImg.dimensions[1]; ++y) {
		CHECK(decompressedImg.pixels[y] == originalImg.pixels[y]);
	}
	delete root;
}

TEST_CASE("Test rescue with single pixel value image") {
	// Test rescue with an image containing only one pixel value
	array<size_t, 2> dims2D = { 10,10 };
	Image<2> originalImg = createSimpleImage(dims2D, static_cast<unsigned char>(123));
	Node* root = buildHuffmanTree(originalImg);
	map<unsigned char, string> huffmanCode;
	generateCodes(root, "", huffmanCode);

	string compressed = compressImage(originalImg, huffmanCode);

	vector<unsigned char> compressedData = bitStringToBytes(compressed);
	auto reverseHuffmanCode = createReverseHuffmanCode(huffmanCode);
	string bitString = bytesToBitString(compressedData);
	Image<2> decompressedImg = createRescueImage(bitString, reverseHuffmanCode, dims2D);

	CHECK(decompressedImg.dimensions[0] == originalImg.dimensions[0]);
	CHECK(decompressedImg.dimensions[1] == originalImg.dimensions[1]);
	for (int y = 0; y < originalImg.dimensions[0] * originalImg.dimensions[1]; ++y) {
		CHECK(decompressedImg.pixels[y] == 123);
	}
	delete root;
}

TEST_CASE("Test rescue with large image") {
	// Test rescue with a large image
	array<size_t, 2> dims2D = { 1000,1000 };
	Image<2> originalImg = createRandomImage(dims2D);
	Node* root = buildHuffmanTree(originalImg);
	map<unsigned char, string> huffmanCode;
	generateCodes(root, "", huffmanCode);

	string compressed = compressImage(originalImg, huffmanCode);

	vector<unsigned char> compressedData = bitStringToBytes(compressed);
	auto reverseHuffmanCode = createReverseHuffmanCode(huffmanCode);
	string bitString = bytesToBitString(compressedData);
	Image<2> decompressedImg = createRescueImage(bitString, reverseHuffmanCode, dims2D);

	CHECK(decompressedImg.dimensions[0] == originalImg.dimensions[0]);
	CHECK(decompressedImg.dimensions[1] == originalImg.dimensions[1]);
	// Check a sample of pixels to ensure correct rescue
	for (int i = 0; i < 100; ++i) {
		int x = rand() % originalImg.dimensions[0];
		int y = rand() % originalImg.dimensions[1];
		CHECK(decompressedImg.pixels[y] == originalImg.pixels[y]);
	}

	delete root;
}

