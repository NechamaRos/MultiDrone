#pragma once
#include <iostream>
#include <vector>
#include "Validation.h"
using namespace std;

class D3Validation :public Validation
{
	vector<vector<vector<vector<uint8_t>>>> validationData;
public:

	D3Validation(const vector<uint8_t>& key) :Validation(key,d3Validation)
	{
	}
	void setValidationData(vector < vector < vector<vector<uint8_t>>>> & validationData) override{ this->validationData = validationData; }
	vector<vector<vector<vector<uint8_t>>>>& getValidationData() { return this->validationData; }

};



