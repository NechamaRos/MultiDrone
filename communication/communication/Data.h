#pragma once
constexpr auto TO_RECYCLE = true;
#include <iostream>
#include "Meta_Data.h"
#include "Message.h"
#include "../Validation/Validation.h"
#include "../Validation/MatValidation.h"
#include "../Validation/VecValidation.h"
#include "../Validation/D3Validation.h"
#include "../Data_Comperession/HuffmanCompression.h"
#include "../Data_Comperession/HuffmanRescue.h"
#include "VecMessage.h"
#include "MatMessage.h"
#include "D3Message.h"
using namespace std;

template<size_t D>
class Data
{
	Meta_Data<D> metaData;
	Message* message=nullptr;
	Message* copyMessage(Message* mes);

	void compressByType();

	void rescueByType();

	void addTheValidation(Message* m, Validation* v);

	void checkTheValidation(Message* m, Validation* v);
public:
	Data(Meta_Data<D>& metaData, Message* message):metaData(metaData), message(message)
	{
		if (checkData(metaData, message)) {
			this->message = copyMessage(message);
		}
		else {
			throw invalid_argument("the fields is not match to each other");
		}
		compressByType();
	}
	Data(Data<D>& data) :metaData(data.metaData), message(data.message ? copyMessage(data.message) : nullptr){}
	~Data() {
		if (this->message != nullptr) {
			delete this->message;
		}
	}
	Data<D>& operator=(Data<D>& data)
	{
		if (this->message) {
			delete this->message;
			if (data.message) {
				this->message = copyMessage(data.message);
			}
			else {
				this->message = nullptr;
			}

		}
		return *this;
	}
	void recycle()
	{
		cout << "recycle!!!" << endl;
	}
	void send()
	{
		Message* m = this->message;
		Validation* v = this->metaData.getValidation();
		try {
			addTheValidation(m, v);
			compressByType();
			cout << "send!!!" << endl;
		}
		catch (exception& e) {
			cout << e.what() << endl;
		}
	}
	void receive()
	{
		Message* m = this->message;
		Validation* v = this->metaData.getValidation();
		try {
			rescueByType();
			checkTheValidation(m, v);
			cout << "receive!!!" << endl;
		}
		catch (runtime_error e) {
			if (TO_RECYCLE) {
				recycle();
			}
		}
		catch (logic_error e) {
			throw e;
		}
	}
	bool checkData(Meta_Data<D>& metaData, Message* message)
	{

		Message* m = message;
		Validation* v = metaData.getValidation();

		if (v->getType() == vecValidation && m->getType() == vecMessage) {
			VecMessage* vm = dynamic_cast<VecMessage*>(m);
			if (v->key.size() - 1 <= log2(vm->getMessage().size())) {
				return true;
			}
		}
		if (v->getType() == matValidation && m->getType() == matMessage) {
			MatMessage* vm = dynamic_cast<MatMessage*>(m);
			if ((v->key.size() - 1) * (vm->getMessage().size() + vm->getMessage()[0].size()) <= log2(vm->getMessage().size() * vm->getMessage()[0].size())) {
				return true;
			}
		}
		if (v->getType() == d3Validation && m->getType() == d3Message) {
			D3Message* vm = dynamic_cast<D3Message*>(m);
			if ((v->key.size() - 1) * (vm->getMessage()[0][0].size() + vm->getMessage()[0].size()) * vm->getMessage().size() <= log2(vm->getMessage().size() * vm->getMessage()[0].size() * vm->getMessage()[0][0].size())) {
				return true;
			}
		}

		return false;
	}
	Meta_Data<D>& getMetaData() { return this->metaData; }
	Message* getMessage() { return this->message; }
};

template<size_t D>
inline Message* Data<D>::copyMessage(Message* mes)
{
	switch (mes->getType())
	{
	case vecMessage: {
		VecMessage* v = dynamic_cast<VecMessage*>(mes);
		VecMessage m = v->getMessage();
		return new VecMessage(m);
		break;
	}
	case matMessage: {
		MatMessage* v = dynamic_cast<MatMessage*>(mes);
		MatMessage m = v->getMessage();
		return new MatMessage(m);
		break;
	}
	case d3Message: {
		D3Message* v = dynamic_cast<D3Message*>(mes);
		D3Message m = v->getMessage();
		return new D3Message(m);
		break;
	}
	default:
		throw invalid_argument("Unknown message type");
	}
	return nullptr;
}
#pragma warning(disable : 4984)

template<size_t D>
inline void Data<D>::compressByType()
{
	if (message == nullptr) {
		throw logic_error("Message is null");
	}
	MessageType type = message->getType();
	switch (type) {
	case vecMessage:
		if constexpr (D == 1) {
			this->metaData.setCompress(new Data_Compression<1>(compressImage(dynamic_cast<VecMessage*>(message)->getMessage())));
		}
		else {
			throw logic_error("Unsupported message type for this Data instance");
		}
		break;
	case matMessage:
		if constexpr (D == 2) {
			this->metaData.setCompress(new Data_Compression<2>(compressImage(dynamic_cast<MatMessage*>(message)->getMessage())));
		}
		else {
			throw logic_error("Unsupported message type for this Data instance");
		}
		break;
	case d3Message:
		if constexpr (D == 3) {
			this->metaData.setCompress(new Data_Compression<3>(compressImage(dynamic_cast<D3Message*>(message)->getMessage())));
		}
		else {
			throw logic_error("Unsupported message type for this Data instance");
		}
		break;
	default:
		throw logic_error("Unsupported message type");
	}
}
#pragma warning(default : 4984)

template<size_t D>
inline void Data<D>::rescueByType()
{
	if (this->message == nullptr)
		throw logic_error("Message is null");

	const Data_Compression<D>* compress = this->metaData.getCompress();
	if (compress == nullptr) {
		throw logic_error("Compress is null");
	}

	Message* newMessage = nullptr;

	switch (compress->getDimension().size()) {
	case 1: {
		const Data_Compression<1>* compress1D = dynamic_cast<const Data_Compression<1>*>(compress);
		if (!compress1D) {
			throw logic_error("Invalid compression type for 1D");
		}
		vector<char> decompressedData = decompressImage(*compress1D);
		newMessage = new VecMessage(decompressedData);
		break;
	}
	case 2: {
		const Data_Compression<2>* compress2D = dynamic_cast<const Data_Compression<2>*>(compress);
		if (!compress2D) {
			throw logic_error("Invalid compression type for 2D");
		}
		vector<vector<uint8_t>> decompressedData = decompressImage(*compress2D);
		newMessage = new MatMessage(decompressedData);
		break;
	}
	case 3: {
		const Data_Compression<3>* compress3D = dynamic_cast<const Data_Compression<3>*>(compress);
		if (!compress3D) {
			throw logic_error("Invalid compression type for 3D");
		}
		vector<vector<vector<uint8_t>>> decompressedData = decompressImage(*compress3D);
		newMessage = new D3Message(decompressedData);
		break;
	}
	default:
		throw logic_error("Unsupported dimension size");
	}

	this->message = newMessage;
}

template<size_t D>
inline void Data<D>::addTheValidation(Message* m, Validation* v)
{
	try {
		switch (v->getType())
		{
		case vecValidation: {
			VecValidation* vv = dynamic_cast<VecValidation*>(v);
			VecMessage* vm = dynamic_cast<VecMessage*>(m);
			auto reminderValue = vm->getReminder(0, vv->key);
			vv->setValidationData(reminderValue);
			break;
		}
		case matValidation: {
			MatValidation* vv = dynamic_cast<MatValidation*>(v);
			MatMessage* vm = dynamic_cast<MatMessage*>(m);
			auto reminderValue = vm->getReminder(0, 0, vv->key);
			vv->setValidationData(reminderValue);
			break;
		}
		case d3Validation: {
			D3Validation* vv = dynamic_cast<D3Validation*>(v);
			D3Message* vm = dynamic_cast<D3Message*>(m);
			auto reminderValue = vm->getReminder(0, 0, 0, vv->key);
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

template<size_t D>
inline void Data<D>::checkTheValidation(Message* m, Validation* v)
{
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
			throw invalid_argument("Unknown message type");
		}
	}
	catch (runtime_error error)
	{
		throw error;
	}
	catch (logic_error e) {
		throw e;
	}
}
