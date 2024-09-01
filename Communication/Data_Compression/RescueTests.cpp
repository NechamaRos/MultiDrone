#include "../Communication/doctest.h"
#include "HuffmanRescue.h"
#include "HuffmanCompression.h"
#include <set>

double calculateImageSimilarity(const Image<2>& img1, const Image<2>& img2) {
	if (img1.pixels.size() != img2.pixels.size()) return 0.0;
	size_t matchingPixels = 0;
	for (size_t i = 0; i < img1.pixels.size(); ++i) {
		if (img1.pixels[i] == img2.pixels[i]) ++matchingPixels;
	}
	return static_cast<double>(matchingPixels) / img1.pixels.size();
}

void testRescue(const Image<2>& originalImg, const Data_Compression<2>& compressed) {
	Image<2> rescuedImg = decompressImage<2>(compressed.getCompressedData(), compressed.getHuffmanCode(), compressed.getDimension());
	CHECK(rescuedImg.dimensions == originalImg.dimensions);
	CHECK(rescuedImg.pixels.size() == originalImg.pixels.size());

	// Allow for small differences due to potential lossy compression
	double similarity = calculateImageSimilarity(originalImg, rescuedImg);
	CHECK(similarity == 1.0);  // Expect 100% similarity
}

void testCompressionAndRescue(const Image<2>& originalImg) {
	try {
		Data_Compression<2> compressed = compressImage(originalImg);
		testRescue(originalImg, compressed);
	}
	catch (const runtime_error& e) {
		CHECK(string(e.what()).find("Cannot compress empty image") != string::npos);
	}
}

TEST_CASE("Huffman Rescue Tests") {

	SUBCASE("Test rescue with large and medium image") {
		// Test rescue with a large image
		array<size_t, 2> dims2D = { 1000, 1000 };
		Image<2> largeImg = createRandomImage(dims2D);
		testCompressionAndRescue(largeImg);

		// Test the entire process of compression followed by rescue
		dims2D = { 50, 50 };
		Image<2> mediumImg = createRandomImage(dims2D);
		testCompressionAndRescue(mediumImg);
	}

	SUBCASE("Test rescue with varying compression ratios") {
		array<size_t, 2> dims2D = { 100, 100 };

		// Test with highly compressible image
		Image<2> highlyCompressibleImg = createSimpleImage(dims2D, static_cast<unsigned char>(0));
		testCompressionAndRescue(highlyCompressibleImg);

		// Test with less compressible image
		Image<2> lessCompressibleImg = createRandomImage(dims2D);
		testCompressionAndRescue(lessCompressibleImg);
	}

	SUBCASE("Test rescue with edge cases") {
		// Test with empty image
		try {
			map<unsigned char, string>huf_code;
			decompressImage<2>("", huf_code, { 2,2 });
		}
		catch (const runtime_error& e) {
			CHECK(string(e.what()).find("Compressed data is too short") != string::npos);
		}
		Image<2> emptyImg;
		testCompressionAndRescue(emptyImg);

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
		//empty image rescue

		array<size_t, 2> dims2D = { 50, 50 };
		Image<2> originalImg = createRandomImage(dims2D);
		Data_Compression<2> compressed = compressImage(originalImg);

		// Corrupt the compressed data
		string corruptedData = compressed.getCompressedData();
		corruptedData[corruptedData.length() / 2] ^= 0xFF;  // Flip all bits in the middle byte
		compressed.setCompressedData(corruptedData);

		double similarity = calculateImageSimilarity(decompressImage<2>(compressed.getCompressedData(), compressed.getHuffmanCode(), dims2D), originalImg);
		CHECK_FALSE(similarity == 1.0);
	}

}
