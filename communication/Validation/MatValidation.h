#pragma once
#include <iostream>
#include <vector>
#include "Validation.h"
using namespace std;

class MatValidation :public Validation
{
	vector< vector< vector<uint8_t>>> validationData;
public:

	MatValidation(const vector<uint8_t>& key) :Validation(key)
	{
	}
	void setValidationData(vector< vector <vector<uint8_t>>>& validationData)override { this->validationData = validationData; }
	vector<vector<vector<uint8_t>>>& getValidationData() { return this->validationData; }

};



