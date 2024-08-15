#include "HuffmanCompression.h"

void buildMinHeap(MinHeap& minHeap,const map<unsigned char, int>& freq_)
{
	// Create a min-heap of all leaf nodes
	for (const auto& pair : freq_) {
		minHeap.push(new Node(pair.first, pair.second));
	}

	// Build the Huffman tree by combining nodes
	while (minHeap.size() > 1) {
		Node* left = minHeap.top(); minHeap.pop();
		Node* right = minHeap.top(); minHeap.pop();

		Node* parent = new Node('\0', left->frequency + right->frequency);
		parent->left = left;
		parent->right = right;

		minHeap.push(parent);
	}
}

//void generateCodes(Node* root, string code, map<unsigned char, string>& huffmanCode) {
//	if (root == nullptr) return;
//
//	// If this is a leaf node, store the code
//	if (!root->left && !root->right) {
//		huffmanCode[root->value] = (code.empty() ? "0" : code); // Ensure every symbol has a code
//	}
//
//	generateCodes(root->left, code + "0", huffmanCode);
//	generateCodes(root->right, code + "1", huffmanCode);
//}

//template<size_t D>
//Node* buildHuffmanTree(const Image<D>& img) {
//	check(img, "Cannot build Huffman tree for empty image");
//	const map<unsigned char, int> freq_ = freq(img);
//	MinHeap minHeap;
//	buildMinHeap(minHeap, freq_);
//
//	// Build the tree
//	while (minHeap.size() > 1) {
//		Node* left = minHeap.top();
//		minHeap.pop();
//		Node* right = minHeap.top();
//		minHeap.pop();
//
//		Node* parent = new Node('\0', left->frequency + right->frequency);
//		parent->left = left;
//		parent->right = right;
//
//		minHeap.push(parent);
//	}
//
//	return minHeap.top();
//}

void generateCodes(Node* root, string code, map<unsigned char, string>& huffmanCode) {
	if (root == nullptr) return;

	// If this is a leaf node, store the code
	if (!root->left && !root->right) {
		huffmanCode[root->value] = code;
	}

	generateCodes(root->left, code + "0", huffmanCode);
	generateCodes(root->right, code + "1", huffmanCode);
}

size_t estimateTreeSize(const map<unsigned char, string>& huffmanCode) {
	// A very rough estimate: 1 byte for the character, 1 byte for code length, plus the code itself
	size_t size = 0;
	for (const auto& pair : huffmanCode) {
		size += 2 + pair.second.length();
	}
	return size * 8; // Convert to bits
}

string runLengthEncode(const vector<unsigned char>& data) {
	string encoded;
	for (size_t i = 0; i < data.size(); ++i) {
		unsigned char count = 1;
		while (i + 1 < data.size() && data[i] == data[i + 1] && count < 255) {
			count++;
			i++;
		}
		encoded += count;
		encoded += data[i];
	}
	return encoded;
}

Data_Compression<1> compressImage(const vector<char>& message)
{
	return compressImage(convertSimpleMessageToImage(message));
}

Data_Compression<2> compressImage(const vector<vector<uint8_t>>& bwImage)
{
	return compressImage(convertBlackWhiteToImage(bwImage));
}

Data_Compression<3> compressImage(const vector<vector<vector<uint8_t>>>& colorImage)
{
	return compressImage(convertColorToImage(colorImage));
}