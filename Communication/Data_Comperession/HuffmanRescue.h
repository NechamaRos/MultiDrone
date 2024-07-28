#pragma once

#include "Image.h"
#include <map>
#include <bitset>
#include <iostream>


/// <summary>
/// Creates a reverse Huffman code map for decoding.
/// </summary>
/// <param name="huffmanCode">The Huffman codes.</param>
/// <returns>A map with the reverse Huffman codes.</returns>
map<string, unsigned char> createReverseHuffmanCode(const map<unsigned char, string>& huffmanCode);

/// <summary>
/// Converts a byte vector to a bit string.
/// </summary>
/// <param name="compressedData">The compressed data in bytes.</param>
/// <returns>A bit string.</returns>
string bytesToBitString(const vector<unsigned char>& compressedData);

/// <summary>
/// Converts a bit string to a byte vector.
/// </summary>
/// <param name="bitString">The bit string to convert.</param>
/// <returns>A vector of bytes.</returns>
vector<unsigned char> bitStringToBytes(const string& bitString);

/// <summary>
/// Initializes an image with the given dimensions.
/// </summary>
/// <typeparam name="D">The dimensionality of the image.</typeparam>
/// <param name="image">The image to be initialized.</param>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The total size of the image in pixels.</returns>
template<size_t D>
int initImage(Image<D>& image, const array<size_t, D>& dims)
{
	image.dimensions = dims;
	size_t totalSize = 1;
	for (size_t dim : dims) {
		totalSize *= dim;
	}
	image.pixels.resize(totalSize);
	return totalSize;
}

/// <summary>
/// Creates a decompressed image from the given bit string and reverse Huffman codes.
/// </summary>
/// <typeparam name="D">The dimensionality of the image.</typeparam>
/// <param name="bitString">The bit string representing the compressed image.</param>
/// <param name="reverseHuffmanCode">The reverse Huffman codes to use for decompression.</param>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The decompressed image.</returns>
template<size_t D>
Image<D> createRescueImage(const string& bitString, const map<string, unsigned char>& reverseHuffmanCode, const array<size_t, D>& dims) {
	Image<D> decompressedImage;
	int totalSize = initImage(decompressedImage, dims);

	string currentCode;
	int pixelIndex = 0;

	if (bitString.empty()) {
		return createRescueEmptyImage(decompressedImage, reverseHuffmanCode);
	}

	for (char bit : bitString) {
		currentCode += bit;
		if (reverseHuffmanCode.find(currentCode) != reverseHuffmanCode.end()) {
			unsigned char pixelValue = reverseHuffmanCode.at(currentCode);
			decompressedImage.pixels[pixelIndex++] = pixelValue;
			currentCode.clear();
			if (pixelIndex >= totalSize) {
				break;
			}
		}
	}

	return decompressedImage;
}

/// <summary>
/// Creates a decompressed image for the case where the compressed bit string is empty.
/// </summary>
/// <typeparam name="D">The dimensionality of the image.</typeparam>
/// <param name="rescueImage">The image to be filled.</param>
/// <param name="reverseHuffmanCode">The reverse Huffman codes containing the pixel value to use.</param>
/// <returns>The decompressed image.</returns>
template<size_t D>
Image<D> createRescueEmptyImage(Image<D>& rescueImage, const map<string, unsigned char>& reverseHuffmanCode)
{
	unsigned char pixelValue = reverseHuffmanCode.begin()->second; // קבלת הערך היחיד במפת הופמן ההפוכה
	fill(rescueImage.pixels.begin(), rescueImage.pixels.end(), pixelValue);
	return rescueImage;
}
