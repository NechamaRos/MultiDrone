#include "Meta_Data.h"
#include "VecValidation.h"
#include "MatValidation.h"
#include "D3Validation.h"

template<typename Base, typename T>
inline bool instanceof(T* t) {
	return dynamic_cast<Base*>(t) != nullptr;
}
Validation* copyValidation(Validation* mes) {
	if (instanceof<VecValidation, Validation>(mes)) {
		VecValidation* v = dynamic_cast<VecValidation*>(mes);
		auto m = v->key;
		return new VecValidation(m);
	}
	else if (instanceof<MatValidation, Validation>(mes)) {
		MatValidation* v = dynamic_cast<MatValidation*>(mes);
		auto m = v->key;
		return new MatValidation(m);
	}
	else {
		D3Validation* v = dynamic_cast<D3Validation*>(mes);
		auto m = v->key;
		return new D3Validation(m);
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
