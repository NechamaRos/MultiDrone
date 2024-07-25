#pragma once
#include <iostream>
#include <vector>
using namespace std;

enum ValidationType
{
	vecValidation, matValidation, d3Validation
};

class Validation
{
	ValidationType type;
public:
	vector<uint8_t> key;

	Validation(const vector<uint8_t>& key,ValidationType type)
		: type(type),key(key)
	{
		if (key.empty()) {
			throw invalid_argument("key is empty");
	}
	}
	ValidationType getType() { return this->type; }
	virtual void setValidationData(vector<vector<vector<vector<uint8_t>>>>& validationData);
	virtual void setValidationData(vector<vector<vector<uint8_t>>> & validationData) ;
	virtual void setValidationData(vector<uint8_t> & validationData) ;

};

