#pragma once
#include "Image.h"
#include "Data_Compression.h"
#include "ImageConversions.h"

/// <summary>
/// Creates a reverse Huffman code map for decoding.
/// </summary>
/// <param name="huffmanCode">The Huffman codes.</param>
/// <returns>A map with the reverse Huffman codes.</returns>
map<string, unsigned char> createReverseHuffmanCode(const map<unsigned char, string>& huffmanCode);

/// <summary>
/// Decompresses Huffman-encoded image data.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="bitString">The bit string representing the compressed image.</param>
/// <param name="reverseHuffmanCode">The reverse Huffman codes to use for decompression.</param>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The decompressed image.</returns>
template<size_t D>
Image<D> decompressHuffman(const string& bitString, const map<string, unsigned char>& reverseHuffmanCode, const array<size_t, D>& dims) {
	Image<D> decompressedImage;
	decompressedImage.initImage(dims);
	string currentCode;
	size_t pixelIndex = 0;

	if (bitString.empty()) {
		return createDecompressEmptyImage(decompressedImage, reverseHuffmanCode.begin()->second);
	}

	for (char bit : bitString) {
		currentCode += bit;
		if (reverseHuffmanCode.find(currentCode) != reverseHuffmanCode.end()) {
			unsigned char pixelValue = reverseHuffmanCode.at(currentCode);
			decompressedImage.pixels[pixelIndex++] = pixelValue;
			currentCode.clear();
			if (pixelIndex >= decompressedImage.pixels.size()) {
				break;
			}
		}
	}
	return decompressedImage;
}

/// <summary>
/// Decompresses RLE-encoded image data.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="rleData">The RLE-encoded image data.</param>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The decompressed image.</returns>
template<size_t D>
Image<D> decompressRLE(const string& rleData, const array<size_t, D>& dims) {
	Image<D> decompressedImage;
	decompressedImage.initImage(dims);
	size_t pixelIndex = 0;

	for (size_t i = 0; i < rleData.length(); i += 2) {
		unsigned char count = rleData[i];
		unsigned char value = rleData[i + 1];
		for (unsigned char j = 0; j < count && pixelIndex < decompressedImage.pixels.size(); ++j) {
			decompressedImage.pixels[pixelIndex++] = value;
		}
	}

	return decompressedImage;
}

/// <summary>
/// Creates a decompressed image for the case where the compressed bit string is empty.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="image">The image to be filled.</param>
/// <param name="pixelValue">The pixel value to use for filling the image.</param>
/// <returns>The decompressed image.</returns>
template<size_t D>
Image<D> createDecompressEmptyImage(Image<D>& image, unsigned char pixelValue) {
	fill(image.pixels.begin(), image.pixels.end(), pixelValue);
	return image;
}

/// <summary>
/// Decompresses the image data based on the compression method used.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="compressedData">The compressed image data.</param>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The decompressed image.</returns>
template <size_t D>
Image<D> decompressImage(const string& compressedData, const map<unsigned char, string>& huffmanCode, const array<size_t, D>& dims) {
	if (compressedData.size() < 3) {
		throw runtime_error("Compressed data is too short");
	}

	string method = compressedData.substr(0, 3);
	string data = compressedData.substr(3);

	try {
		if (method == "HUR") {
			// Huffman & RLE compression 
			Image<D> rleCompression = decompressRLE(data, dims);
			map<string, unsigned char> reverseHuffmanCode = createReverseHuffmanCode(huffmanCode);
			cout << "Huffman & RLE rescue \n";
			return decompressHuffman(bytesToBitString(rleCompression.pixels), reverseHuffmanCode, dims);
		}
		else if (method == "HUF") {
			// Huffman compression
			map<string, unsigned char> reverseHuffmanCode = createReverseHuffmanCode(huffmanCode);
			cout << "Huffman rescue \n";
			return decompressHuffman(data, reverseHuffmanCode, dims);
		}
		else if (method == "RLE") {
			// RLE compression
			cout << "RLE rescue \n";
			return decompressRLE(data, dims);
		}
		else if (method == "RAW") {
			// No compression
			Image<D> image;
			image.initImage(dims);
			if (data.size() != image.pixels.size()) {
				throw runtime_error("Raw data size mismatch");
			}
			cout << "RAW rescue \n";
			copy(data.begin(), data.end(), image.pixels.begin());
			return image;
		}
		else {
			throw runtime_error("Unknown compression method: " + method);
		}
	}
	catch (const exception& e) {
		throw runtime_error("Error during decompression: " + string(e.what()));
	}
}

vector<char> decompressImage(const Data_Compression<1>& compress);

vector<vector<uint8_t>> decompressImage(const Data_Compression<2>& compress);

vector<vector<vector<uint8_t>>> decompressImage(const Data_Compression<3>& compress);
