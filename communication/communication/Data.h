#pragma once
#include <iostream>
#include "Meta_Data.h"
#include "Message.h"
#include "../Validation/Validation.h"
#include "../Validation/MatValidation.h"
#include "../Validation/VecValidation.h"
#include "../Validation/D3Validation.h"
#include "VecMessage.h"
#include "MatMessage.h"
#include "D3Message.h"
using namespace std;
class Data
{
	Meta_Data metaData;
	Message* message=nullptr;
	
public:
	Data(Meta_Data& metaData, Message* message);
	Data(Data& data);
	~Data() {
		if (this->message != nullptr) {
			delete this->message;
		}
	}
	Data& operator=(Data& data);
	void send();
	void receive();
	bool checkData(Meta_Data& metaData, Message* message);
	Meta_Data& getMetaData() { return this->metaData; }
	Message* getMessage() { return this->message; }
};

