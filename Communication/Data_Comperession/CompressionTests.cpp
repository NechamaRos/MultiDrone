
#include "doctest.h"
#include "HuffmanCompression.h"


TEST_CASE("buildHuffmanTree tests") {
	SUBCASE("Test with empty image") {
		// Ensure that an exception is thrown for an empty image
		Image<2> emptyImg;
		CHECK_THROWS_AS(buildHuffmanTree(emptyImg), runtime_error);
	}

	SUBCASE("Test with uniform pixel value image") {
		// Test tree building with an image containing a single pixel value
		array<size_t, 2> dims2D = { 10,10 };
		Image<2> uniformImg = createSimpleImage(dims2D, static_cast<unsigned char>(100));
		Node* root = buildHuffmanTree(uniformImg);
		CHECK(root != nullptr);
		CHECK(root->value == 100);
		CHECK(root->frequency == 100);
		delete root;
	}

	SUBCASE("Test with diverse pixel values") {
		// Test tree building with an image containing diverse pixel values
		array<size_t, 2> dims2D = { 4,4 };
		Image<2> diverseImg = createRandomImage(dims2D);
		Node* root = buildHuffmanTree(diverseImg);
		CHECK(root != nullptr);
		CHECK(root->frequency == 16);
		delete root;
	}
}

TEST_CASE("generateHuffmanCodes tests") {
	SUBCASE("Test with simple Huffman tree") {
		// Test code generation for a simple tree with two leaves
		Node* root = new Node('\0', 2);
		root->left = new Node('A', 1);
		root->right = new Node('B', 1);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		CHECK(huffmanCode['A'] == "0");
		CHECK(huffmanCode['B'] == "1");
		root->left = root->right = nullptr;
		delete root;
	}

	SUBCASE("Test with complex Huffman tree") {
		// Test code generation for a more complex tree
		Node* root = new Node('\0', 100);
		root->left = new Node('\0', 60);
		root->right = new Node('\0', 40);
		root->left->left = new Node('A', 30);
		root->left->right = new Node('B', 30);
		root->right->left = new Node('C', 20);
		root->right->right = new Node('D', 20);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		CHECK(huffmanCode['A'] == "00");
		CHECK(huffmanCode['B'] == "01");
		CHECK(huffmanCode['C'] == "10");
		CHECK(huffmanCode['D'] == "11");

		CHECK(huffmanCode.size() == 4);

		// Verify prefix-free property
		for (const auto& code1 : huffmanCode) {
			for (const auto& code2 : huffmanCode) {
				if (code1.first != code2.first) {
					CHECK_FALSE(code1.second.find(code2.second) == 0);
				}
			}
		}

		// Clean up
		root->left->left = root->left->right = nullptr;
		root->right->left = root->right->right = nullptr;
		root->left = root->right = nullptr;
		delete root;
	}

	SUBCASE("Verify prefix-free property") {
		// Ensure that no code is a prefix of another code
		array<size_t, 2> dims2D = { 4,4 };
		Image<2> diverseImg = createRandomImage(dims2D);
		Node* root = buildHuffmanTree(diverseImg);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);

		for (const auto& code1 : huffmanCode) {
			for (const auto& code2 : huffmanCode) {
				if (code1.first != code2.first) {
					CHECK_FALSE(code1.second.find(code2.second) == 0);
					CHECK_FALSE(code2.second.find(code1.second) == 0);
				}
			}
		}

		delete root;
	}
}

TEST_CASE("compressImage tests") {
	SUBCASE("Test with small, known image") {
		// Test compression with a small, predictable image
		array<size_t, 2> dims2D = { 2,2 };
		Image<2> smallImg = createSampleImage(dims2D);
		Node* root = buildHuffmanTree(smallImg);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		string compressed = compressImage(smallImg, huffmanCode);
		CHECK(compressed.length() > 0);
		delete root;
	}

	SUBCASE("Verify compressed data is shorter than original") {
		// Ensure that compression actually reduces data size
		array<size_t, 2> dims2D = { 100,100 };
		Image<2> img = createRandomImage(dims2D);
		Node* root = buildHuffmanTree(img);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		string compressed = compressImage(img, huffmanCode);
		CHECK(compressed.length() < 100 * 100 * 8);
		delete root;
	}

	SUBCASE("Test with large image") {
		// Test compression with a large image
		array<size_t, 2> dims2D = { 1000,1000 };
		Image<2> largeImg = createRandomImage(dims2D);
		Node* root = buildHuffmanTree(largeImg);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		string compressed = compressImage(largeImg, huffmanCode);
		CHECK(compressed.length() > 0);
		delete root;
	}
}

TEST_CASE("Integration tests") {
	SUBCASE("Test full process: tree building to compression") {
		// Test the entire compression process using a sample image
		array<size_t, 2> dims2D = { 64, 64 };
		Image<2> sampleImg = createSampleImage(dims2D);

		Node* root = buildHuffmanTree(sampleImg);
		REQUIRE(root != nullptr);

		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		REQUIRE(!huffmanCode.empty());

		string compressed = compressImage(sampleImg, huffmanCode);
		REQUIRE(!compressed.empty());

		// Check if compression actually reduced the size
		CHECK(compressed.length() < sampleImg.dimensions[0] * sampleImg.dimensions[1] * 8);

		double compressionRatio = static_cast<double>(sampleImg.dimensions[0] * sampleImg.dimensions[1] * 8) / compressed.length();

		delete root;
	}
}

TEST_CASE("Edge case tests") {
	SUBCASE("Test with very large image") {
		// Test compression with a very large image
		array<size_t, 2> dims2D = { 5000,5000 };
		Image<2> veryLargeImg = createRandomImage(dims2D);
		Node* root = buildHuffmanTree(veryLargeImg);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		string compressed = compressImage(veryLargeImg, huffmanCode);
		CHECK(compressed.length() > 0);
		delete root;
	}

	SUBCASE("Test with 3 dimension of image") {
		// Test compression with a very large image
		array<size_t, 3> dims3D = { 10,10,3 };
		Image<3> veryLargeImg = createRandomImage(dims3D);
		Node* root = buildHuffmanTree(veryLargeImg);
		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		string compressed = compressImage(veryLargeImg, huffmanCode);
		CHECK(compressed.length() > 0);
		delete root;
	}

	SUBCASE("Test with single pixel value image") {
		// Test compression with an image containing only one pixel value
		array<size_t, 2> dims2D = { 1,1 };
		Image<2> singleValueImg = createSimpleImage(dims2D, static_cast<unsigned char>(123));

		Node* root = buildHuffmanTree(singleValueImg);
		CHECK(root != nullptr);

		map<unsigned char, string> huffmanCode;
		generateCodes(root, "", huffmanCode);
		CHECK(huffmanCode.size() == 1);

		string compressed = compressImage(singleValueImg, huffmanCode);
		CHECK(compressed.empty());

		delete root;
	}
}
