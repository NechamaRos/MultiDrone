#pragma once
#include <iostream>
#include <vector>
#include "Message.h"
using namespace std;
class VecMessage:public Message
{
	vector<char> message;
public:

	VecMessage(const vector<char>& message)
		:Message(vecMessage), message(message)
	{
	}

	// Inherited via Message
	vector<char>& getMessage() { return this->message; }
	vector<uint8_t> getreminder(int, vector<uint8_t>&) override;
	void checkInReceive(vector<uint8_t>& valiDationData, vector<uint8_t>& key);
};

