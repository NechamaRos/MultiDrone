#pragma once
#include <future>
#include <vector>
#include <string>
#include "../Communication/Meta_Data.h" 

using namespace std;

class TransferData {
public:
    int num_cores();
    void waiting(vector<future<bool>>& futures);
    bool sendMessageByChunk(const string& chunk);
    bool sendMetaData(const Meta_Data& metaData);
    bool sendData(const string& data, const Meta_Data& metaData);
    void SendsAsynchronously(const string& str, const Meta_Data& metaData, size_t numChunks, size_t chunk_size, size_t numThreads);
    void SendsSynchronously(const string& str, const Meta_Data& metaData);
    int choosing_an_option_to_transfer();
    void preparingTheDataForTransferring(const string& str, const Meta_Data& metaData);
};
