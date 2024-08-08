#pragma once
#include <future>
#include <vector>
#include <map>
#include <string>
#include "../Communication/Meta_Data.h" 
constexpr size_t MAX_DATA_SIZE_FOR_THREAD = 4;//4 bytes 1048576=1mb
constexpr size_t OPTION_TO_SEND = 2; //sends Asynchronously
using namespace std;

class TransferData {
public:
    int num_cores();
    void waiting(vector<future<bool>>& futures);
<<<<<<< HEAD
    bool sendMessageByChunk(const string& chunk,int sockfd);
    bool sendMetaData(const Meta_Data& metaData, int client_sockfd);
    bool sendData(const string& data, const Meta_Data& metaData, int client_sockfd);
    void sendsAsynchronously(const string& dataAsStr, const Meta_Data& metaData, size_t numChunks, size_t chunk_size, size_t numThreads, int client_sockfd);
    void sendsSynchronously(const string& dataAsStr, const Meta_Data& metaData, int client_socket);
    void preparingTheDataForTransferring(const string& dataAsStr, const Meta_Data& metaData, int client_sockfd);
=======
    bool sendMessageByChunk(const string& chunk, size_t chunkIndex);
    bool sendMetaData(const Meta_Data& metaData);
    bool sendData(const string& data, const Meta_Data& metaData);
    void sendsAsynchronously(const string& dataAsStr, const Meta_Data& metaData, size_t numChunks, size_t chunk_size, size_t numThreads);
    void sendsSynchronously(const string& dataAsStr, const Meta_Data& metaData);
    void preparingTheDataForTransferring(const string& dataAsStr, const Meta_Data& metaData);

    void addChunk(const string& chunk, size_t chunkIndex);
    string getCollectedData();

private:
    map<size_t, string> collectedDataMap;
    mutex dataMutex;
>>>>>>> team_Communication
};
