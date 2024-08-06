
#include <iostream>
#include <vector>
using namespace std;

#pragma once
class CRCAlgorithm
{
	void Xor(vector<uint8_t>& arr1, const vector<uint8_t>& arr2);

	vector<uint8_t> divideModulo2(const vector<uint8_t>& toDiv, const  vector<uint8_t>& divBy);
public:
	void copyArr(const vector<uint8_t>& src, vector<uint8_t>& dest, size_t len);

	vector<uint8_t> getRemainder(const vector<uint8_t>& data, vector<uint8_t>& key);

	vector<uint8_t> getDataWithRemainder(const vector<uint8_t>& data, vector<uint8_t>& key);

	void receiver(const vector<uint8_t>& data, vector<uint8_t>& key);

};

