#include "D3Message.h"



bool D3Message::checkMAt(int height, int width, const vector<vector<uint8_t>>& mat)
{
	if (mat.size() != height)
		return true;
	for (size_t i = 0; i < height; i++)
	{
		if (mat[i].size() != width) {
			return true;
		}
	}
	return false;
}

D3Message::D3Message(const vector<vector<vector<uint8_t>>>& message)throw(logic_error)
	: message(message)
{
	int len = message.size();
	int height = message[0].size();
	int width = message[0][0].size();
	for (size_t i = 0; i < len; i++)
	{
		if (checkMAt(height, width, message[i])) {
			throw logic_error("message must includ equal size matrixes");
		}
	}
}

vector<vector<vector<vector<uint8_t>>>> D3Message::getreminder(int, int, int, vector<uint8_t>& key)throw (exception)
{
	int rows = this->message.size();
	vector<vector<vector<vector<uint8_t>>>> reminder(rows);
	try
	{
		for (int i = 0; i < rows; i++) {
			MatMessage mm(this->message[i]);
			reminder[i] = mm.getreminder(0, 0, key);
		}
		return reminder;

	}
	catch (const std::exception& e)
	{
		throw e;
	}
	
}

void D3Message::checkInReceive(vector<vector<vector<vector<uint8_t>>>>& valiDationData, vector<uint8_t>& key)throw (runtime_error)
{
	int size = this->message.size();
	for (int i = 0; i < size; i++) {
		MatMessage mm(this->message[i]);
		try
		{
			mm.checkInReceive(valiDationData[i], key);
		}
		catch (runtime_error error)
		{
			throw error;
		}
	}
}




