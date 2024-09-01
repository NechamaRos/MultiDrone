#pragma once
#include <future>
#include <vector>
#include <map>
#include <string>
#include <iostream>          
#include <thread>
#include <chrono>
#include <bitset>
#include <stdexcept> // This for std::runtime_error
#include "../Communication/Meta_Data.h" 
constexpr size_t MAX_DATA_SIZE_FOR_THREAD = 4;//4 bytes 1048576=1mb
constexpr size_t OPTION_TO_SEND = 2; //sends Asynchronously
using namespace std;

class TransferData {
public:
	int num_cores();
	void waiting(vector<future<bool>>& futures);
	bool sendMessageByChunk(const string& chunk, size_t chunkIndex);
	template<size_t D>
	bool sendMetaData(const Meta_Data<D>& metaData)
	{
		try
		{
			//Noa function
			this_thread::sleep_for(chrono::milliseconds(100));
			cout << "Sent metaData validation " << endl; //<< metaData 
			return true;
		}
		catch (const exception&)
		{
			throw runtime_error("The send failed");
		}
	}
	template<size_t D>
	bool sendData(const string& data, const Meta_Data<D>& metaData) {
		try
		{
			if (data.empty()) {
				throw runtime_error("Data string is empty");
			}
			//Noa function
			return true;
		}
		catch (const exception&)
		{
			throw runtime_error("The send failed");
		}
	}
	template<size_t D>
	void sendsAsynchronously(const string& dataAsStr, const Meta_Data<D>& metaData, size_t numChunks, size_t chunk_size, size_t numThreads)
	{
		if (dataAsStr.empty()) {
			throw std::runtime_error("Data is empty");
		}
		size_t str_size = dataAsStr.size();
		//open the threads and send to socket function.
		vector<future<bool>> futures;
		size_t chunkStart = 0;
		for (size_t i = 0; i < numChunks; ++i) {
			size_t chunkEnd = min(chunkStart + chunk_size, str_size);
			string chunk = dataAsStr.substr(chunkStart, chunkEnd - chunkStart);
			chunkStart = chunkEnd;
			// If we have reached the maximum number of threads, wait for one to finish
			if (futures.size() >= numThreads) {
				for (auto it = futures.begin(); it != futures.end();) {
					if (it->wait_for(chrono::seconds(0)) == future_status::ready) {
						it = futures.erase(it);
					}
					else {
						++it;
					}
				}
			}
			// Submit the block in a new thread
			futures.push_back(async(launch::async, &TransferData::sendMessageByChunk, this, chunk, i));
		}
		futures.push_back(async(launch::async, &TransferData::sendMetaData<D>, this, std::cref(metaData)));
		// Wait for all remaining threads to finish
		waiting(futures);
	}
	template<size_t D>
	void sendsSynchronously(const string& dataAsStr, const Meta_Data<D>& metaData)
	{
		if (dataAsStr.empty()) {
			throw runtime_error("Data is empty");
		}
		this->sendData(dataAsStr, metaData);
	}
	template<size_t D>
	void preparingTheDataForTransferring(const string& dataAsStr, const Meta_Data<D>& metaData)
	{
		//count of threads.
		const int num_cores = this->num_cores(); // The maximum number of threads

		//found size of every chunk.
		size_t str_size = dataAsStr.size();
		size_t chunk_size = str_size / num_cores;
		if (chunk_size > MAX_DATA_SIZE_FOR_THREAD) {
			chunk_size = MAX_DATA_SIZE_FOR_THREAD;
		}
		//count of number chunks.
		size_t numChunks = (str_size + chunk_size - 1) / chunk_size;

		//choose the option to send Async / sync
		size_t option = OPTION_TO_SEND;
		if (option == 1)
		{
			sendsSynchronously(dataAsStr, metaData);
		}
		else
			sendsAsynchronously(dataAsStr, metaData, numChunks, chunk_size, num_cores);
	}
	void addChunk(const string& chunk, size_t chunkIndex);
	string getCollectedData();

private:
	map<size_t, string> collectedDataMap;
	mutex dataMutex;
};
