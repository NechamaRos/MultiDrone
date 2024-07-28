#include "HuffmanCompression.h"


void printHuffmanTree(Node* bt) {
	cout << "\n";
	for (int i = 0; i <= 3; i++)
	{
		printByLevel(bt, i);
		cout << "\n";
	}
}

void printByLevel(Node* bt, int level)
{
	if (bt != nullptr)
	{
		if (level == 0)
			cout << bt->frequency << " ";
		else
		{
			printByLevel(bt->left, level - 1);
			printByLevel(bt->right, level - 1);
		}
	}
}

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

void generateCodes(Node* root, string code, map<unsigned char, string>& huffmanCode) {
	if (root == nullptr) return;

	// If this is a leaf node, store the code
	if (!root->left && !root->right) {
		huffmanCode[root->value] = code;
	}

	generateCodes(root->left, code + "0", huffmanCode);
	generateCodes(root->right, code + "1", huffmanCode);
}

