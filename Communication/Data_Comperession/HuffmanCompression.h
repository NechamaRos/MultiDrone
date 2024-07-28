#pragma once

#include "Image.h"
#include "HuffmanNode.h"
#include <iostream>
#include <queue>
#include <map>

/// <summary>
/// Represents a priority queue of Node pointers sorted by CompareNode criteria.
/// </summary>
typedef priority_queue<Node*, vector<Node*>, CompareNode> MinHeap;

/// <summary>
/// Prints the Huffman tree for debugging purposes.
/// </summary>
/// <param name="bt">The root of the Huffman tree to print.</param>
void printHuffmanTree(Node* bt);

/// <summary>
/// Helper function for the printHuffmanTree function
/// </summary>
/// <param name="bt">The root of the Huffman tree to print.</param>
/// <param name="level">The level of the node in the tree</param>
void printByLevel(Node* bt, int level);

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
/// <typeparam name="D">The dimensionality of the image.</typeparam>
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
/// <typeparam name="D">The dimensionality of the image.</typeparam>
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
/// <typeparam name="D">The dimensionality of the image.</typeparam>
/// <param name="img">The image from which to build the Huffman tree.</param>
/// <returns>The root node of the Huffman tree.</returns>
template<size_t D>
Node* buildHuffmanTree(const Image<D>& img) {
	check(img, "Cannot build Huffman tree for empty image");

	const map<unsigned char, int> freq_ = freq(img);

	MinHeap minHeap;
	buildMinHeap(minHeap, freq_);

	printHuffmanTree(minHeap.top());
	return minHeap.top();
}

/// <summary>
/// Compresses the image using the given Huffman codes.
/// </summary>
/// <typeparam name="D">The dimensionality of the image.</typeparam>
/// <param name="img">The image to be compressed.</param>
/// <param name="huffmanCode">The Huffman codes to use for compression.</param>
/// <returns>The compressed image as a string of bits.</returns>
template<size_t D>
string compressImage(const Image<D>& img, const map<unsigned char, string>& huffmanCode)
{
	check(img, "Cannot compress empty image");

	string compressed;

	if (huffmanCode.size() == 1) {
		compressed = "";
	}
	else {
		for (unsigned char pixel : img.pixels) {
			compressed += huffmanCode.at(pixel);
		}
	}

	return compressed;
}


