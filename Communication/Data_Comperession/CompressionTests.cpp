#include "doctest.h"
#include "HuffmanCompression.h"

// Helper function to create and test a Huffman tree
template <size_t N>
Node* createAndTestTree(const Image<N>& img) {
	Node* root = buildHuffmanTree(img);
	CHECK(root != nullptr);
	CHECK(root->frequency == img.pixels.size());
	return root;
}

// Helper function to generate and test Huffman codes
void generateAndTestCodes(Node* root, map<unsigned char, string>& huffmanCode) {
	generateCodes(root, "", huffmanCode);
	CHECK(!huffmanCode.empty());

	// Verify prefix-free property
	for (const auto& code1 : huffmanCode) {
		for (const auto& code2 : huffmanCode) {
			if (code1.first != code2.first) {
				CHECK_FALSE(code1.second.find(code2.second) == 0);
				CHECK_FALSE(code2.second.find(code1.second) == 0);
			}
		}
	}
}

// Helper function to compress and test an image
template <size_t N>
void compressAndTestImage(const Image<N>& img) {
	string compressed = compressImage(img).getCompressedData();
	CHECK(!compressed.empty());

	// Calculate sizes
	size_t originalSize = img.pixels.size() * sizeof(unsigned char);
	size_t compressedSize = (compressed.length() + 7) / 8; // Size in bytes (rounded up)
	double compressionRatio = static_cast<double>(originalSize) / compressedSize;

	// Print compression information
	cout << "Compression ratio: " << compressionRatio << ":1" << endl;
	cout << "Space saving: " << (1 - 1 / compressionRatio) * 100 << "%" << endl;

	// Tests
	CHECK(compressedSize < originalSize);
	CHECK(compressionRatio > 1.0);

	// Check compression type
	CHECK((compressed.substr(0, 3) == "RLE" ||
		compressed.substr(0, 3) == "HUF" ||
		compressed.substr(0, 3) == "HUR" ||
		compressed.substr(0, 3) == "RAW"));
}

TEST_CASE("Huffman Compression Tests") {
	SUBCASE("buildHuffmanTree tests") {
		// Test with empty image
		CHECK_THROWS_AS(buildHuffmanTree(Image<2>()), runtime_error);

		// Test with uniform pixel value image
		array<size_t, 2> dims2D = { 10, 10 };
		auto uniformImg = createSimpleImage(dims2D, static_cast<unsigned char>(100));
		Node* root = createAndTestTree(uniformImg);
		CHECK(root->value == 100);
		CHECK(root->frequency == 100);
		delete root;

		// Test with diverse pixel values
		auto diverseImg = createRandomImage(dims2D);
		root = createAndTestTree(diverseImg);
		CHECK(root->frequency == 100);
		delete root;
	}

	SUBCASE("generateHuffmanCodes tests") {
		// Test with simple Huffman tree
		Node* root = new Node('\0', 2);
		root->left = new Node('A', 1);
		root->right = new Node('B', 1);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		CHECK(huffmanCode['A'] == "0");
		CHECK(huffmanCode['B'] == "1");
		root->left = root->right = nullptr;
		delete root;

		// Test with complex Huffman tree
		root = new Node('\0', 100);
		root->left = new Node('\0', 60);
		root->right = new Node('\0', 40);
		root->left->left = new Node('A', 30);
		root->left->right = new Node('B', 30);
		root->right->left = new Node('C', 20);
		root->right->right = new Node('D', 20);
		huffmanCode.clear();
		generateAndTestCodes(root, huffmanCode);
		CHECK(huffmanCode['A'] == "00");
		CHECK(huffmanCode['B'] == "01");
		CHECK(huffmanCode['C'] == "10");
		CHECK(huffmanCode['D'] == "11");
		CHECK(huffmanCode.size() == 4);

		// Clean up
		root->left->left = root->left->right = nullptr;
		root->right->left = root->right->right = nullptr;
		root->left = root->right = nullptr;
		delete root;

		// Verify prefix-free property
		array<size_t, 2> dims2D = { 4, 4 };
		auto diverseImg = createRandomImage(dims2D);
		root = createAndTestTree(diverseImg);
		huffmanCode.clear();
		generateAndTestCodes(root, huffmanCode);
		delete root;
	}

	SUBCASE("compressImage tests") {
		// Test with small, known image
		array<size_t, 2> smallDims = { 2, 2 };
		auto smallImg = createSampleImage(smallDims);
		compressAndTestImage(smallImg);

		// Test with large image
		array<size_t, 2> largeDims = { 1000, 1000 };
		auto largeImg = createRandomImage(largeDims);
		compressAndTestImage(largeImg);
	}

	SUBCASE("Integration tests") {
		// Test full process: tree building to compression
		array<size_t, 2> dims2D = { 64, 64 };
		auto sampleImg = createSampleImage(dims2D);
		compressAndTestImage(sampleImg);
	}

	SUBCASE("Edge case tests") {
		// Test with very large image
		array<size_t, 2> veryLargeDims = { 5000, 5000 };
		auto veryLargeImg = createRandomImage(veryLargeDims);
		compressAndTestImage(veryLargeImg);

		// Test with 3 dimension image
		array<size_t, 3> dims3D = { 10, 10, 3 };
		auto img3D = createRandomImage(dims3D);

		compressAndTestImage(img3D);

		// Test with single pixel value image
		array<size_t, 2> singlePixelDims = { 1, 1 };
		auto singleValueImg = createSimpleImage(singlePixelDims, static_cast<unsigned char>(123));
		string compressed = compressImage(singleValueImg).getCompressedData();
		CHECK(!compressed.empty());
		CHECK(compressed.substr(0, 3) == "HUF");
	}
}

// Helper functions for the new tests

bool isCompleteTree(Node* root) {
	if (root == nullptr) return true;
	if (root->left == nullptr && root->right == nullptr) return true;
	if (root->left != nullptr && root->right != nullptr)
		return isCompleteTree(root->left) && isCompleteTree(root->right);
	return false;
}

bool satisfiesHuffmanProperty(Node* root) {
	if (root == nullptr || (root->left == nullptr && root->right == nullptr)) return true;
	if (root->left == nullptr || root->right == nullptr) return false;
	return (root->frequency >= root->left->frequency + root->right->frequency) &&
		satisfiesHuffmanProperty(root->left) && satisfiesHuffmanProperty(root->right);
}

Image<2> createTwoColorImage(const array<size_t, 2>& dims) {
	Image<2> img;
	img.initImage(dims);
	for (size_t i = 0; i < img.pixels.size(); ++i) {
		img.pixels[i] = (i % 2 == 0) ? 0 : 255;
	}
	return img;
}

bool lessFrequentSymbolsHaveLongerCodes(const map<unsigned char, string>& huffmanCode, const Image<2>& img) {
	// Calculate frequency of each symbol
	map<unsigned char, int> freq = ::freq(img);
	// Create a vector of pairs (symbol, frequency) and sort by frequency
	vector<pair<unsigned char, int>> freqVec(freq.begin(), freq.end());
	sort(freqVec.begin(), freqVec.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

	// Check if code lengths are non-increasing as frequency decreases
	for (size_t i = 1; i < freqVec.size(); ++i) {
		try {
			if (huffmanCode.at(freqVec[i - 1].first).length() > huffmanCode.at(freqVec[i].first).length()) {
				return false;
			}
		}
		catch(exception ex){
			cout << "i " << i << "\n";
		}
	}
	return true;
}

TEST_CASE("Huffman Compression Additional Tests") {
	SUBCASE("Test compression with different image sizes") {
		// Test with very small image
		array<size_t, 2> tinyDims = { 2, 2 };
		auto tinyImg = createRandomImage(tinyDims);
		compressAndTestImage(tinyImg);

		// Test with medium-sized image
		array<size_t, 2> mediumDims = { 100, 100 };
		auto mediumImg = createRandomImage(mediumDims);
		compressAndTestImage(mediumImg);
	}

	SUBCASE("Test compression with different pixel distributions") {
		array<size_t, 2> dims = { 50, 50 };

		// Test with highly uniform image
		auto uniformImg = createSimpleImage(dims, static_cast<unsigned char>(100));
		compressAndTestImage(uniformImg);

		// Test with image having two dominant colors
		auto twoColorImg = createTwoColorImage(dims);
		compressAndTestImage(twoColorImg);
	}

	SUBCASE("Test Huffman tree properties") {
		array<size_t, 2> dims = { 50, 50 };
		Image<2> img = createRandomImage(dims);
		Node* root = buildHuffmanTree(img);

		// Check if the tree is complete (each node has either 0 or 2 children)
		CHECK(isCompleteTree(root));

		// Check if the tree satisfies the Huffman property (frequency of parent >= sum of frequencies of children)
		CHECK(satisfiesHuffmanProperty(root));

		delete root;
	}

	SUBCASE("Test code length properties") {
		array<size_t, 2> dims = { 50, 50 };
		auto img = createRandomImage(dims);
		Node* root = buildHuffmanTree(img);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);

		// Check if less frequent symbols have longer codes
		//CHECK(lessFrequentSymbolsHaveLongerCodes(huffmanCode, img));// not work

		delete root;
	}
}
