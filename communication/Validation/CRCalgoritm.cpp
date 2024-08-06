#include "CRCalgoritm.h"
void CRCAlgorithm::Xor(vector<uint8_t>& arr1, const vector<uint8_t>& arr2) {
	size_t len = arr1.size();
	if (arr2.size() != len)
		return;
	for (size_t i = 1; i < len; i++) {
		if (arr1[i] == arr2[i])
			arr1[i - 1] = 0;
		else
			arr1[i - 1] = 1;
	}
}
void CRCAlgorithm::copyArr(const vector<uint8_t>& src, vector<uint8_t>& dest, size_t len) {
	if (src.empty()) {
		return;
	}
	if (src.size() < len) {
		throw invalid_argument("len is bigger than source");
	}
	for (size_t i = 0; i < len; i++) {
		dest.push_back(src[i]);
	}
}
vector<uint8_t> CRCAlgorithm::divideModulo2(const vector<uint8_t>& toDiv, const  vector<uint8_t>& divBy) {
	const size_t lenToDiv = toDiv.size();
	const size_t lenDivBy = divBy.size();
	if (lenToDiv < lenDivBy)
		throw runtime_error("can not div small parameter");
	for (size_t i = 0; i < lenDivBy; i++)
		if (divBy[i] != 1 && divBy[i] != 0) {
			throw runtime_error("must send vectors that contains only `1` or `0`");
		}
	for (size_t i = 0; i < lenToDiv; i++)
		if (toDiv[i] != 1 && toDiv[i] != 0) {
			throw runtime_error("must send vectors that contains only `1` or `0`");
		}
	size_t point = lenDivBy;
	vector<uint8_t> zeros;
	for (size_t i = 0; i < lenDivBy; i++) {
		zeros.push_back(0);
	}
	vector<uint8_t> tmp;
	copyArr(toDiv, tmp, lenDivBy);
	while (point < lenToDiv) {
		if (tmp[0] == 1) {
			Xor(tmp, divBy);
			tmp[lenDivBy - 1] = toDiv[point];
		}
		else {
			Xor(tmp, zeros);
			tmp[lenDivBy - 1] = toDiv[point];
		}
		point++;
	}
	if (tmp[0] = 1) {
		Xor(tmp, divBy);
	}
	else {
		Xor(tmp, zeros);
	}
	vector<uint8_t> res;
	copyArr(tmp, res, lenDivBy - 1);
	return res;
}

vector<uint8_t> CRCAlgorithm::getRemainder(const vector<uint8_t>& data, vector<uint8_t>& key) {
	if (key.empty())
		throw invalid_argument("Invalid key");
	size_t len_data = data.size();
	size_t len_key = key.size();
	vector<uint8_t> dataTocalc;
	copyArr(data, dataTocalc, len_data);
	vector<uint8_t> zeros;
	for (size_t i = 0; i < len_key - 1; i++) {
		zeros.push_back(0);
	}
	copyArr(zeros, dataTocalc, len_key - 1);
	vector<uint8_t> remainder;
	try {
		 remainder = divideModulo2(dataTocalc, key);
	}
	catch(runtime_error err){
		throw err;

	}
	return remainder;
}
vector<uint8_t> CRCAlgorithm::getDataWithRemainder(const vector<uint8_t>& data, vector<uint8_t>& key) {
	vector<uint8_t> rem = getRemainder(data, key);
	vector<uint8_t> dataWithRemainder;
	copyArr(data, dataWithRemainder, data.size());
	copyArr(rem, dataWithRemainder, rem.size());
	return dataWithRemainder;
}
void CRCAlgorithm::receiver(const vector<uint8_t>& data, vector<uint8_t>& key) {
	if (key.empty())
		throw invalid_argument("Invalid key");
	vector<uint8_t> remainder;
	try
	{
		 remainder = divideModulo2(data, key);
	}
	catch (runtime_error err)
	{
		throw err;
	}

	size_t len = remainder.size();
	for (size_t i = 0; i < len; i++)
		if (remainder[i] == 1)
			throw runtime_error("error in received data");

}