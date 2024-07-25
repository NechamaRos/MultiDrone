#include "MatMessage.h"
#include "../Validation/CRCalgoritm.h"

void MatMessage::copyCol(vector<uint8_t>& arr, int numCol) {
	int len = this->message.size();
	if (numCol >= this->message[0].size()) {
		throw invalid_argument("invalid num col");
	}
	for (int i = 0; i < len; i++) {
		arr.push_back(this->message[i][numCol]);
	}
}


vector<vector<vector<uint8_t>>> MatMessage::getreminder(int, int, vector<uint8_t>& key)throw(exception)
{
	int rows = this->message.size(),cols=this->message[0].size();
	vector<vector<vector<uint8_t>>> res(2);
	vector<vector<uint8_t>> rowsReminder(rows);
	vector<vector<uint8_t>> colsReminder(cols);
	vector<uint8_t> oneCol(rows);
	CRCalgoritm crc;

	for (int i = 0; i < rows; i++) {
		try
		{
			rowsReminder[i] = crc.getRemainder(this->message[i], key);
		}
		catch (const exception& e)
		{
			throw e;
		}
	}

	for (int j = 0; j < cols; j++) {
		oneCol.clear();
		copyCol(oneCol, j);
		colsReminder[j]=crc.getRemainder(oneCol, key);
	}
	res[0] = rowsReminder;
	res[1] = colsReminder;

	return res;
}

void MatMessage::checkInReceive(vector<vector<vector<uint8_t>>>& valiDationData, vector<uint8_t>& key)throw(runtime_error)
{
	CRCalgoritm crc;
	vector<uint8_t> tmp;
	int lenKey = key.size();
	int rows = valiDationData[0].size();
	int cols = valiDationData[1].size();

	for (int i = 0; i < rows; i++) {
		tmp.clear();
		tmp = this->message[i];
		crc.copyArr(valiDationData[0][i], tmp, lenKey-1);
		try
		{
			crc.receiver(tmp, key);
		}
		catch (runtime_error error)
		{
			throw error;
		}
	}

	for (int i = 0; i < cols; i++) {
		tmp.clear();
		copyCol(tmp, i);
		crc.copyArr(valiDationData[1][i], tmp, lenKey-1);
		try
		{
			crc.receiver(tmp, key);
		}
		catch (runtime_error error)
		{
			throw error;
		}
	}
}
