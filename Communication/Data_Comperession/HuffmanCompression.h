#pragma once
#include <algorithm>
#include <queue>
#include <map>
#include "HuffmanNode.h"
#include"Data_Compression.h"
#include "ImageConversions.h"

/// <summary>
/// Represents a priority queue of Node pointers sorted by CompareNode criteria.
/// </summary>
typedef priority_queue<Node*, vector<Node*>, CompareNode> MinHeap;

/// <summary>
/// Builds a minimum heap from the given frequency map.
/// </summary>
/// <param name="minHeap">The minHeap to be built.</param>
/// <param name="freq_">The frequency map of pixel values.</param>
void buildMinHeap(MinHeap& minHeap, const map<unsigned char, int>& freq_);

/// <summary>
/// Generates the Huffman codes from the Huffman tree.
/// </summary>
/// <param name="root">The root of the Huffman tree.</param>
/// <param name="code">The current Huffman code being generated.</param>
/// <param name="huffmanCode">The map to store the generated Huffman codes.</param>
void generateCodes(Node* root, string code, map<unsigned char, string>& huffmanCode);

/// <summary>
/// Calculates the frequency of each pixel value in the image.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="img">The image for which to calculate the frequencies.</param>
/// <returns>A map containing the frequency of each pixel value.</returns>
template<size_t D>
map<unsigned char, int> freq(const Image<D>& img) {
	map<unsigned char, int> freq_;

	// Calculate the frequency of each pixel value
	for (const auto& pixel : img.pixels) {
		freq_[pixel]++;
	}

	return freq_;
}

/// <summary>
/// Checks if the image is empty and throws an exception if it is.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="img">The image to be checked.</param>
/// <param name="message">The error message to be thrown if the image is empty.</param>
template<size_t D>
void check(const Image<D>& img, const string& message) {
	// Check for an empty image
	size_t totalSize = 1;
	for (size_t dim : img.dimensions) {
		totalSize *= dim;
	}
	if (totalSize == 0 || img.pixels.empty()) {
		throw runtime_error(message);
	}
}

/// <summary>
/// Builds a Huffman tree from the given image.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="img">The image from which to build the Huffman tree.</param>
/// <returns>The root node of the Huffman tree.</returns>
template<size_t D>
Node* buildHuffmanTree(const Image<D>& img) {
	check(img, "Cannot build Huffman tree for empty image");

	const map<unsigned char, int> freq_ = freq(img);

	MinHeap minHeap;
	buildMinHeap(minHeap, freq_);

	return minHeap.top();
}

/// <summary>
/// Estimates the size of the Huffman tree based on the Huffman codes.
/// </summary>
/// <param name="huffmanCode">A map of pixel values to their Huffman codes.</param>
/// <returns>An estimate of the Huffman tree size in bytes.</returns>
size_t estimateTreeSize(const map<unsigned char, string>& huffmanCode);

/// <summary>
/// Performs Run-Length Encoding (RLE) on the input data.
/// </summary>
/// <param name="data">The vector of unsigned chars to be encoded.</param>
/// <returns>A string containing the RLE-encoded data.</returns>
string runLengthEncode(const vector<unsigned char>& data);

/// <summary>
/// Determines the best compression method among Huffman, RLE, and RAW (no compression).
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="huffmanCompressed">The Huffman-compressed string.</param>
/// <param name="rleCompressed">The RLE-compressed string.</param>
/// <param name="img">The original image.</param>
/// <returns>
/// A string with a 3-character prefix indicating the compression method used (HUF, RLE, or RAW),
/// followed by the compressed data.
/// </returns>
template<size_t D>
string bestCompression(const string& huffmanCompressed, const string& rleCompressed, const Image<D>& img) {
	size_t rawSize = img.pixels.size();
	size_t huffmanSize = huffmanCompressed.size();
	size_t rleSize = rleCompressed.size();

	// Convert Huffman compressed string to bytes and apply RLE
	vector<unsigned char> huffmanCompressedBytes = bitStringToBytes(huffmanCompressed);
	string rleHufCompression = runLengthEncode(huffmanCompressedBytes);
	size_t rleHufSize = rleHufCompression.size();

	// Find the smallest size
	//size_t minSize = min({ rawSize, huffmanSize, rleSize, rleHufSize });
	array<size_t, 4> sizes = { rawSize, huffmanSize, rleSize, rleHufSize };
	size_t minSize = *min_element(sizes.begin(), sizes.end());

	if (minSize == rawSize) {
		cout << "RAW compression\n";
		return "RAW" + string(img.pixels.begin(), img.pixels.end());
	}
	else if (minSize == huffmanSize) {
		cout << "HUF compression\n";
		return "HUF" + huffmanCompressed;
	}
	else if (minSize == rleSize) {
		cout << "RLE compression\n";
		return "RLE" + rleCompressed;
	}
	else {
		cout << "HUF & RLE compression\n";
		return "HUR" + rleHufCompression;
	}
}
/// <summary>
/// Compresses the image
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="img">The image to be compressed.</param>
/// <returns>The compressed image as a string of bits.</returns>
template<size_t D>
Data_Compression<D> compressImage(const Image<D>& img)
{
	check(img, "Cannot compress empty image");
	Node* root = buildHuffmanTree(img);
	map<unsigned char, string> huffmanCode;
	generateCodes(root, "", huffmanCode);

	// Try Huffman compression
	string huffmanCompressed;
	if (huffmanCode.size() == 1)
		// Single color image, can be represented very efficiently
		huffmanCompressed = "";
	else
		for (unsigned char pixel : img.pixels) {
			huffmanCompressed += huffmanCode.at(pixel);
		}

	// Try RLE compression
	string rleCompressed = runLengthEncode(img.pixels);

	Data_Compression<D> compressedData(bestCompression(huffmanCompressed, rleCompressed, img));
	compressedData.setDimension(img.dimensions);
	if (compressedData.getCompressedData().substr(0, 2) == "HU") {// If compression chosen is HUF || HUF && RLE
		compressedData.addHuffmanCode(huffmanCode);
	}
	return compressedData;
}

Data_Compression<1> compressImage(const vector<char>& message);

Data_Compression<2> compressImage(const vector<vector<uint8_t>>& bwImage);

Data_Compression<3> compressImage(const vector<vector<vector<uint8_t>>>& colorImage);
