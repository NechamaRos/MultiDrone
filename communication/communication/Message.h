#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
using namespace std;

enum MessageType
{
	vecMessage, matMessage, d3Message
};

class Message
{
	MessageType type;

public:

	virtual vector<vector<vector<vector<uint8_t>>>> getReminder(size_t, size_t, size_t, vector<uint8_t>&);
	virtual vector<vector<vector<uint8_t>>> getReminder(size_t, size_t, vector<uint8_t>&);
	virtual vector<uint8_t> getReminder(size_t, vector<uint8_t>&);
	Message(const MessageType& type);
	MessageType getType() const { return this->type; }
};

