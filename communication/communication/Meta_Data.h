#pragma once
#include "../Validation/Validation.h"
class Meta_Data
{
	Validation* validation=nullptr;
public:

	Meta_Data(Validation* validation);
	Meta_Data(Meta_Data& md);
	~Meta_Data() {
		if (this->validation != nullptr) {
			delete this->validation;
		}
	}
	Meta_Data& operator=(Meta_Data& md);
	Validation* getValidation() { return this->validation; }
};

