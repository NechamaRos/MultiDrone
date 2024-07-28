#include "HuffmanRescue.h"

map<string, unsigned char> createReverseHuffmanCode(const map<unsigned char, string>& huffmanCode) {
	map<string, unsigned char> reverseCode;
	for (const auto& pair : huffmanCode) {
		reverseCode[pair.second] = pair.first;
	}
	return reverseCode;
}

string bytesToBitString(const vector<unsigned char>& compressedData) {
	string bitString;
	for (unsigned char byte : compressedData) {
		bitString += bitset<8>(byte).to_string();
	}
	return bitString;
}

vector<unsigned char> bitStringToBytes(const string& bitString) {

	vector<unsigned char> bytes;
	string paddedBitString = bitString;

	// Add zeros to the end of the string to make its length a multiple of 8
	while (paddedBitString.length() % 8 != 0) {
		paddedBitString += "0";
	}

	for (size_t i = 0; i < paddedBitString.length(); i += 8) {
		string byte = paddedBitString.substr(i, 8);

		// Check if the string is valid for conversion
		if (byte.find_first_not_of("01") != string::npos) {
			cout << "Invalid bit string: " << byte << "\n";
			throw invalid_argument("Invalid bit string: " + byte);
		}

		bytes.push_back(static_cast<unsigned char>(stoi(byte, nullptr, 2)));
	}

	return bytes;
}
