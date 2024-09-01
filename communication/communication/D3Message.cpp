#include "D3Message.h"

bool D3Message::checkMAt(size_t height, size_t width, const vector<vector<uint8_t>>& mat)
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

D3Message::D3Message(const vector<vector<vector<uint8_t>>>& message)
	:Message(d3Message), message(message)
{
	size_t len = message.size();
	size_t height = message[0].size();
	size_t width = message[0][0].size();
	for (size_t i = 0; i < len; i++)
	{
		if (checkMAt(height, width, message[i])) {
			throw logic_error("message must include equal size matrixes");
		}
	}
}

vector<vector<vector<vector<uint8_t>>>> D3Message::getReminder(size_t, size_t, size_t, vector<uint8_t>& key)
{
	size_t rows = this->message.size();
	vector<vector<vector<vector<uint8_t>>>> reminder(rows);
	try
	{
		for (size_t i = 0; i < rows; i++) {
			MatMessage mm(this->message[i]);
			reminder[i] = mm.getReminder(0, 0, key);
		}
		return reminder;

	}
	catch (const  exception& e)
	{
		throw e;
	}
	
}

void D3Message::checkInReceive(vector<vector<vector<vector<uint8_t>>>>& valiDationData, vector<uint8_t>& key)
{
	size_t size = this->message.size();
	for (size_t i = 0; i < size; i++) {
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




