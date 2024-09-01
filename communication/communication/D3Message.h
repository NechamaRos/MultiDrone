#pragma once
#include <iostream>
#include <vector>
#include "Message.h"
#include "MatMessage.h"
using namespace std;




class D3Message :public Message
{
	vector<vector<vector<uint8_t>>> message;
	bool checkMAt(size_t height, size_t width, const vector<vector<uint8_t>>& mat);
public:
	D3Message(const vector<vector<vector<uint8_t>>>& message);
	vector<vector<vector<uint8_t>>>& getMessage() { return this->message; }

//	// Inherited via Message
	vector<vector<vector<vector<uint8_t>>>> getReminder(size_t, size_t, size_t, vector<uint8_t>&) override;
	void checkInReceive(vector<vector<vector<vector<uint8_t>>>>& valiDationData, vector<uint8_t>& key);

};

