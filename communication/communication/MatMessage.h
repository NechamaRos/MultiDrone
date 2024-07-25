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
		int height = message.size();
		int width = message[0].size();
		for (size_t i = 1; i < height; i++)
		{
			if (message[i].size() != width)
				throw logic_error("message must be a matrix");
		}
	}
	vector<vector<uint8_t>>&  getMessage() { return this->message; }
	void copyCol(vector<uint8_t>& arr, int numCol);
	// Inherited via Message
	vector<vector<vector<uint8_t>>> getreminder(int, int, vector<uint8_t>&) override;
	void checkInReceive(vector<vector<vector<uint8_t>>>& valiDationData, vector<uint8_t>& key);

};

