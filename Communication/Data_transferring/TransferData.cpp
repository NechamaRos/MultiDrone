#include <iostream>     
#include <future>      
#include <thread>
#include <chrono>
#include <vector>
#include <bitset>
#include <stdexcept>
#include "TransferData.h"
#include "../Communication/Meta_Data.h"


using namespace std;

int TransferData::num_cores() {
    unsigned int numCores = std::thread::hardware_concurrency();
    std::cout << "Number of cores: " << numCores << std::endl;
    return numCores;
}
void TransferData::waiting(vector<future<bool>>& futures) {
    for (auto& future : futures) {
        bool result = future.get();
        std::cout << "Result: " << std::boolalpha << result << std::endl;
    }
}
bool TransferData::sendMessageByChunk(const string& chunk, size_t chunkIndex) {
    try {
        if (chunk.empty()) {
            throw std::runtime_error("Chunk is empty");
        }
        //Noa function
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Sent chunk: " << chunk << std::endl;

        addChunk(chunk, chunkIndex);

        return true;
    }
    catch (const std::exception&) {
        throw std::runtime_error("The send failed");
    }
}
bool TransferData::sendMetaData(const Meta_Data& metaData)
{
    
    try
    {
        //Noa function
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        std::cout << "Sent metaData validation " << std::endl; //<< metaData 
        return true;
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("The send failed");
    }

}
bool TransferData::sendData(const string& data, const Meta_Data& metaData) {
    try
    {
        if (data.empty()) {
            throw std::runtime_error("Data string is empty");
        }
        //Noa function
        return true;
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("The send failed");
    }
}
void TransferData::sendsAsynchronously(const string& dataAsStr, const Meta_Data& metaData, size_t numChunks, size_t chunk_size, size_t numThreads)
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
    futures.push_back(async(launch::async, &TransferData::sendMetaData, this, std::cref(metaData)));
    // Wait for all remaining threads to finish
    waiting(futures);
}
void TransferData::sendsSynchronously(const string& dataAsStr, const Meta_Data& metaData)
{
    if (dataAsStr.empty()) {
        throw std::runtime_error("Data is empty");
    }
    this->sendData(dataAsStr, metaData);
}
void TransferData::preparingTheDataForTransferring(const string& dataAsStr, const Meta_Data& metaData)
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

    //choose the obtion to send Async / sync
    size_t option = OPTION_TO_SEND;
    if (option == 1)
    {
        sendsSynchronously(dataAsStr, metaData);
    }
    else
        sendsAsynchronously(dataAsStr, metaData, numChunks, chunk_size, num_cores);
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
