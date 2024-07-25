#pragma once
#include <iostream>
#include <vector>
#include "Validation.h"
using namespace std;

class VecValidation:public Validation
{
	vector<uint8_t> validationData;
public:

	VecValidation( const vector<uint8_t>& key):Validation(key,vecValidation)
	{
	}
	void setValidationData(vector<uint8_t>& validationData)override {this->validationData=validationData;}
	vector<uint8_t>& getValidationData() { return this->validationData; }
};

