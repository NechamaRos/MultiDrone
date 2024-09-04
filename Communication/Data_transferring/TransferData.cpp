
#include <iostream>     
#include <future>      
#include <thread>
#include <chrono>
#include <vector>
#include <bitset>
#include <stdexcept>
#include "TransferData.h"
#include "../Communication/Meta_Data.h"
#include "../Socket_communication/DroneCommunicationManager.h"
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

bool TransferData::sendMessageByChunk(const string& chunk, size_t chunkIndex, int num_drone) {
    try
    {
        if (chunk.empty()) {
            throw std::runtime_error("Chunk is empty");
        }
        int result= droneCommunicationManager->send_message_to_drone(num_drone, chunk.c_str());
        //Add verification of whether the data was sent
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Sent chunk: " << chunk << std::endl;

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
