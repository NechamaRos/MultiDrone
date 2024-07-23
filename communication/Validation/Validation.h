#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Validation
{
public:
	vector<uint8_t> key;

	Validation(const vector<uint8_t>& key)
		: key(key)
	{
		if (key.empty()) {
			throw invalid_argument("key is empty");
	}
	}
	virtual void setValidationData(vector<vector<vector<vector<uint8_t>>>>& validationData);
	virtual void setValidationData(vector<vector<vector<uint8_t>>> & validationData) ;
	virtual void setValidationData(vector<uint8_t> & validationData) ;

};

