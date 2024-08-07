#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
using namespace std;

enum MessageType
{
	vecMessage,matMessage,d3Message
};

class Message
{
	MessageType type;

public:

	virtual vector<vector<vector<vector<uint8_t>>>> getreminder(int ,int,int,  vector<uint8_t>&) ;
	virtual vector<vector<vector<uint8_t>>> getreminder(int,int,  vector<uint8_t>&) ;
	virtual vector<uint8_t> getreminder(int,vector<uint8_t>&) ;
	Message(const MessageType& type);
	MessageType getType() { return this->type; }
};

