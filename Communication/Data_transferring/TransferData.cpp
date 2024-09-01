#include "TransferData.h"

using namespace std;

int TransferData::num_cores() {
	unsigned int numCores = thread::hardware_concurrency();
	cout << "Number of cores: " << numCores << endl;
	return numCores;
}

void TransferData::waiting(vector<future<bool>>& futures) {
	for (auto& future : futures) {
		bool result = future.get();
		cout << "Result: " << boolalpha << result << endl;
	}
}

bool TransferData::sendMessageByChunk(const string& chunk, size_t chunkIndex) {
	try {
		if (chunk.empty()) {
			throw  runtime_error("Chunk is empty");
		}
		//Noa function, open the socket.
		this_thread::sleep_for(chrono::milliseconds(100));
		cout << "Sent chunk: " << chunk << endl;
		addChunk(chunk, chunkIndex);

		return true;
	}
	catch (const  exception&)
	{
		throw runtime_error("The send failed");
	}
}

void TransferData::addChunk(const string& chunk, size_t chunkIndex) {
	lock_guard<mutex> lock(dataMutex);
	collectedDataMap[chunkIndex] = chunk;
}

string TransferData::getCollectedData() {
	lock_guard<mutex> lock(dataMutex);
	string collectedData;
	for (const auto& pair : collectedDataMap) {
		collectedData += pair.second;
	}
	return collectedData;
}
