#pragma once
#include <iostream>
#include <vector>
#include "Message.h"
using namespace std;

class MatMessage:public Message
{
	vector<vector<uint8_t>> message;

public:

	MatMessage(const vector<vector<uint8_t>>& message)
		:Message(matMessage), message(message)
	{
		size_t height = message.size();
		size_t width = message[0].size();
		for (size_t i = 1; i < height; i++)
		{
			if (message[i].size() != width)
				throw logic_error("message must be a matrix");
		}
	}
	vector<vector<uint8_t>>&  getMessage() { return this->message; }
	void copyCol(vector<uint8_t>& arr, size_t numCol);
	// Inherited via Message
	vector<vector<vector<uint8_t>>> getReminder(size_t, size_t, vector<uint8_t>&) override;
	void checkInReceive(vector<vector<vector<uint8_t>>>& valiDationData, vector<uint8_t>& key);

};

