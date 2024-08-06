#pragma once
#include <string>
#include <map>
#include <array>
#include<iostream>
using namespace std;

template<size_t D>
struct Data_Compression
{
private:
	string compressedData;
	map<unsigned char, string> huffmanCode;
	array<size_t, D> dims;
public:
	Data_Compression(string compressedData) :compressedData(compressedData), dims{} {}

	// Copy constructor
	Data_Compression(const Data_Compression<D>& other)
		: compressedData(other.compressedData), huffmanCode(other.huffmanCode), dims(other.dims) {
	}

	// Copy assignment operator
	Data_Compression<D>& operator=(const Data_Compression<D>& other) {
		if (this != &other) {
			compressedData = other.compressedData;
			huffmanCode = other.huffmanCode;
			dims = other.dims;
		}
		return *this;
	}

	// Move constructor
	Data_Compression(Data_Compression<D>&& other) noexcept
		: compressedData(move(other.compressedData)), huffmanCode(move(other.huffmanCode)), dims(move(other.dims)) {
	}

	// Move assignment operator
	Data_Compression<D>& operator=(Data_Compression<D>&& other) noexcept {
		if (this != &other) {
			compressedData = move(other.compressedData);
			huffmanCode = move(other.huffmanCode);
			dims = move(other.dims);
		}
		return *this;
	}

	// Virtual destructor to ensure polymorphic behavior
	virtual ~Data_Compression() = default;

	const string& getCompressedData() const { return compressedData; }

	void setCompressData(const string& compress) { compressedData = compress; }

	void addHuffmanCode(const map<unsigned char, string>& huffmanCode) { this->huffmanCode = huffmanCode; }

	const map<unsigned char, string>& getHuffmanCode() const { return this->huffmanCode; }

	const array<size_t, D>& getDimension() const { return this->dims; }

	const size_t size() const {
		size_t dims = 1;
		for (size_t i = 0; i < D; i++)
		{
			dims *= this->dims[i];
		}
		return dims;
	}

	void setDimension(const array<size_t, D> dims) {
		for (size_t i = 0; i < D; i++)
		{
			this->dims[i] = dims[i];
		}
	}
};
