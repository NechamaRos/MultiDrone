#include "Data.h"

Message* copyMessage(Message* mes) {
	switch (mes->getType())
	{
	case vecMessage:{
		VecMessage* v = dynamic_cast<VecMessage*>(mes);
		auto m = v->getMessage();
		return new VecMessage(m);
		break;
	}
	case matMessage:{
		MatMessage* v = dynamic_cast<MatMessage*>(mes);
		auto m = v->getMessage();
		return new MatMessage(m);
		break;
	}
	case d3Message:{
		D3Message* v = dynamic_cast<D3Message*>(mes);
		auto m = v->getMessage();
		return new D3Message(m);
		break;
	}
	default:
		break;
	}
}

Data::Data(Meta_Data& metaData, Message* message)throw(invalid_argument) :metaData(metaData)  {
		
	if (checkData(metaData, message)) {
			this->message = copyMessage(message); 
	}
	else {
		throw invalid_argument("the fields is not match to each other");
	}
}

Data::Data(Data& data):metaData(data.metaData),message(nullptr)
{
	if (data.message) {
		this->message = copyMessage(data.message);
	}
}

Data& Data::operator=(Data& data)
{
	if (this->message) {
		delete this-> message;
		if (data.message) {
			this->message = copyMessage(data.message);
		}
		else {
			this->message = nullptr;
		}

	}
	return *this;
}

void addTheValidation(Message* m, Validation* v) {
	try {
		switch (v->getType())
		{
		case vecValidation: {
			VecValidation* vv = dynamic_cast<VecValidation*>(v);
			VecMessage* vm = dynamic_cast<VecMessage*>(m);
			auto reminderValue = vm->getreminder(0, vv->key);
			vv->setValidationData(reminderValue);
			break;
		}
		case matValidation: {
			MatValidation* vv = dynamic_cast<MatValidation*>(v);
			MatMessage* vm = dynamic_cast<MatMessage*>(m);
			auto reminderValue = vm->getreminder(0, 0, vv->key);
			vv->setValidationData(reminderValue);
			break;
		}
		case d3Validation: {
			D3Validation* vv = dynamic_cast<D3Validation*>(v);
			D3Message* vm = dynamic_cast<D3Message*>(m);
			auto reminderValue = vm->getreminder(0, 0, 0, vv->key);
			vv->setValidationData(reminderValue);
			break;
		}
		default:
			break;
		}
	}
	catch (exception& e) {
		throw e;
	}
}

void Data::send()
{	
	Message* m = this->message;
	Validation* v = this->metaData.getValidation();
	try {
		addTheValidation(m, v);
		cout << "send!!!" << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
}

void checkTheValidation(Message* m, Validation* v)throw (logic_error){
	try {
		switch (v->getType())
		{
		case vecValidation: {
			VecValidation* vv = dynamic_cast<VecValidation*>(v);
			VecMessage* vm = dynamic_cast<VecMessage*>(m);
			if (vv->getValidationData().empty()) {
				throw logic_error("the data validation is empty");
			}
			vm->checkInReceive(vv->getValidationData(), vv->key);
			break;
		}
		case matValidation: {
			MatValidation* vv = dynamic_cast<MatValidation*>(v);
			MatMessage* vm = dynamic_cast<MatMessage*>(m);
			if (vv->getValidationData().empty()) {
				throw logic_error("the data validation is empty");
			}
			vm->checkInReceive(vv->getValidationData(), vv->key);
			break;
		}
		case d3Validation: {
			D3Validation* vv = dynamic_cast<D3Validation*>(v);
			D3Message* vm = dynamic_cast<D3Message*>(m);
			if (vv->getValidationData().empty()) {
				throw logic_error("the data validation is empty");
			}
			vm->checkInReceive(vv->getValidationData(), vv->key);
			break;
		}
		default:
			break;
		}
	}
	catch (runtime_error error)
	{
		cout << error.what() << endl;
	}
	catch (logic_error e) {
		throw e;
	}
}

void Data::receive()throw (logic_error)
{
	Message* m = this->message;
	Validation* v = this->metaData.getValidation();
	try{
		checkTheValidation(m, v);
		cout << "receive!!!" << endl;
	}
	catch (logic_error e) {
		throw e;
	}
}

bool Data::checkData(Meta_Data& metaData, Message* message)
{

	Message* m = message;
	Validation* v = metaData.getValidation();

	if (v->getType() == vecValidation && m->getType()== vecMessage){
		VecMessage* vm = dynamic_cast<VecMessage*>(m);
		if (v->key.size()-1 <= log2(vm->getMessage().size())) {
			return true;
		}
	}
	if (v->getType() == matValidation && m->getType() == matMessage) {
		MatMessage* vm = dynamic_cast<MatMessage*>(m);
		if ((v->key.size()-1)*(vm->getMessage().size() + vm->getMessage()[0].size()) <= log2(vm->getMessage().size()* vm->getMessage()[0].size())) {
			return true;
		}
	}
	if (v->getType() == d3Validation && m->getType() == d3Message) {
		D3Message* vm = dynamic_cast<D3Message*>(m);
		if ((v->key.size() - 1) * (vm->getMessage()[0][0].size() + vm->getMessage()[0].size())*vm->getMessage().size() <= log2(vm->getMessage().size() * vm->getMessage()[0].size()* vm->getMessage()[0][0].size())) {
			return true;
		}
	}
		
	return false;
}
