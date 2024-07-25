#include "Data.h"



template<typename Base, typename T>
inline bool instanceof(T* t) {
	return dynamic_cast<Base*>(t)!=nullptr;
}
Message* copyMessage(Message* mes) {
	if (instanceof<VecMessage, Message>(mes)) {
		VecMessage* v = dynamic_cast<VecMessage*>(mes);
		auto m = v->getMessage();
		return new VecMessage(m);
	}
	else if (instanceof<MatMessage, Message>(mes)) {
		MatMessage* v = dynamic_cast<MatMessage*>(mes);
		auto m = v->getMessage();
		return new MatMessage(m);
	}
	else {
		D3Message* v = dynamic_cast<D3Message*>(mes);
		auto m = v->getMessage();
		return new D3Message(m);
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

void Data::send()
{	
	Message* m = this->message;
	Validation* v = this->metaData.getValidation();
	try {
		if (instanceof<VecValidation>(v)) {
			VecValidation* vv = dynamic_cast<VecValidation*>(v);
			VecMessage* vm = dynamic_cast<VecMessage*>(m);
			auto reminderValue = vm->getreminder(0, vv->key);
			vv->setValidationData(reminderValue);
		}
		else if (instanceof<MatValidation>(v)) {
			MatValidation* vv = dynamic_cast<MatValidation*>(v);
			MatMessage* vm = dynamic_cast<MatMessage*>(m);
			auto reminderValue = vm->getreminder(0, 0, vv->key);
			vv->setValidationData(reminderValue);
		}
		else {
			D3Validation* vv = dynamic_cast<D3Validation*>(v);
			D3Message* vm = dynamic_cast<D3Message*>(m);
			auto reminderValue = vm->getreminder(0, 0, 0, vv->key);
			vv->setValidationData(reminderValue);
		}
		cout << "send!!!" << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
}

void Data::receive()throw (logic_error)
{
	Message* m = this->message;
	Validation* v = this->metaData.getValidation();

	if (instanceof<VecValidation>(v)) {
		VecValidation* vv = dynamic_cast<VecValidation*>(v);
		VecMessage* vm = dynamic_cast<VecMessage*>(m);
		try
		{
			if (vv->getValidationData().empty()) {
				throw logic_error("the data validation is empty");
			}
			vm->checkInReceive(vv->getValidationData(), vv->key);
			cout << "receive!!!" << endl;
		}
		catch (runtime_error error)
		{

			cout << error.what() << endl;
		}
		catch (logic_error e) {
			throw e;
		}
	}
	else if (instanceof<MatValidation>(v)) {
		MatValidation* vv = dynamic_cast<MatValidation*>(v);
		MatMessage* vm = dynamic_cast<MatMessage*>(m);
		try
		{
			if (vv->getValidationData().empty()) {
				throw logic_error("the data validation is empty");
			}
			vm->checkInReceive(vv->getValidationData(), vv->key);
			cout << "receive!!!" << endl;

		}
		catch (runtime_error error)
		{
			cout << error.what() << endl;
		}
		catch (logic_error e) {
			throw e;
		}
	}
	else {
		D3Validation* vv = dynamic_cast<D3Validation*>(v);
		D3Message* vm = dynamic_cast<D3Message*>(m);
		try
		{
			if (vv->getValidationData().empty()) {
				throw logic_error("the data validation is empty");
			}
			vm->checkInReceive(vv->getValidationData(), vv->key);
			cout << "receive!!!" << endl;
		}
		catch (runtime_error error)
		{
			cout << error.what() << endl;
		}
		catch (logic_error e) {
			throw e;
		}
	}
}

bool Data::checkData(Meta_Data& metaData, Message* message)
{

	Message* m = message;
	Validation* v = metaData.getValidation();

	if (instanceof<VecValidation, Validation>(v) && instanceof<VecMessage,Message>(m)) {
		VecMessage* vm = dynamic_cast<VecMessage*>(m);
		if (v->key.size()-1 <= log2(vm->getMessage().size())) {
			return true;
		}
	}
	if (instanceof<MatValidation>(v) && instanceof<MatMessage>(m)) {
		MatMessage* vm = dynamic_cast<MatMessage*>(m);
		if ((v->key.size()-1)*(vm->getMessage().size() + vm->getMessage()[0].size()) <= log2(vm->getMessage().size()* vm->getMessage()[0].size())) {
			return true;
		}
	}
	if (instanceof<D3Validation>(v) && instanceof<D3Message>(m)) {
		D3Message* vm = dynamic_cast<D3Message*>(m);
		if ((v->key.size() - 1) * (vm->getMessage()[0][0].size() + vm->getMessage()[0].size())*vm->getMessage().size() <= log2(vm->getMessage().size() * vm->getMessage()[0].size()* vm->getMessage()[0][0].size())) {
			return true;
		}
	}
		
	return false;
}
