#pragma once
#include "../Validation/Validation.h"
#include "../Data_Comperession/Data_Compression.h"
#include "../Validation/VecValidation.h"
#include "../Validation/MatValidation.h"
#include "../Validation/D3Validation.h"

template<size_t D>
class Meta_Data
{
	Validation* validation = nullptr;
	Data_Compression<D>* compress = nullptr;
	Validation* copyValidation(Validation* mes);

public:
	Meta_Data(Validation* validation) :validation(nullptr), compress(nullptr)
	{
		this->validation = copyValidation(validation);
	}
	Meta_Data(Meta_Data<D>& md) :validation(nullptr), compress(nullptr)
	{
		if (md.validation)
			this->validation = copyValidation(md.validation);
		if (md.compress) {
			this->compress = new Data_Compression<D>(*md.compress);
		}
	}
	~Meta_Data() {
		if (this->validation != nullptr) {
			delete this->validation;
		}
		if (this->compress != nullptr) {
			delete this->compress;
		}
	}

	Meta_Data<D>& operator=(const Meta_Data<D>& md) {
		if (this != &md) {
			delete validation;
			delete compress;
			validation = md.validation ? copyValidation(md.validation) : nullptr;
			compress = md.compress ? new Data_Compression<D>(*md.compress) : nullptr;
		}
		return *this;
	}

	Validation* getValidation() { return this->validation; }

	void setCompress(Data_Compression<D>* compress) {
		this->compress = compress;
	}

	const Data_Compression<D>* getCompress() const {
		return compress;
	}
};

template<size_t D>
inline Validation* Meta_Data<D>::copyValidation(Validation* mes)
{
	switch (mes->getType())
	{
	case vecValidation: {
		VecValidation* v = dynamic_cast<VecValidation*>(mes);
		VecValidation m = v->key;
		return new VecValidation(m);
		break;
	}
	case matValidation: {
		MatValidation* v = dynamic_cast<MatValidation*>(mes);
		MatValidation m = v->key;
		return new MatValidation(m);
		break;
	}
	case d3Validation: {
		D3Validation* v = dynamic_cast<D3Validation*>(mes);
		D3Validation m = v->key;
		return new D3Validation(m);
		break;
	}
	default:
		throw invalid_argument("Unknown message type");
	}
	return nullptr;
}
