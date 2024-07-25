#include "Meta_Data.h"
#include "../Validation/VecValidation.h"
#include "../Validation/MatValidation.h"
#include "../Validation/D3Validation.h"

Validation* copyValidation(Validation* mes) {
	switch (mes->getType())
	{
	case vecValidation: {
		VecValidation* v = dynamic_cast<VecValidation*>(mes);
		auto m = v->key;
		return new VecValidation(m);
		break;
	}
	case matValidation: {
		MatValidation* v = dynamic_cast<MatValidation*>(mes);
		auto m = v->key;
		return new MatValidation(m);
		break;
	}
	case d3Validation: {
		D3Validation* v = dynamic_cast<D3Validation*>(mes);
		auto m = v->key;
		return new D3Validation(m);
		break;
	}
	default:
		break;
	}
}
Meta_Data::Meta_Data(Validation* validation)
{
	this->validation = copyValidation(validation);
}

Meta_Data::Meta_Data(Meta_Data& md):validation(nullptr)
{
	if(md.validation)
		this->validation = copyValidation(md.validation);
}

Meta_Data& Meta_Data::operator=(Meta_Data& md)
{
	if (this != &md) {
		delete this->validation;
		if (md.validation) {
			this->validation = copyValidation(md.validation);
		}
		else {
			this->validation = nullptr;
		}
	}
	return *this;
}
