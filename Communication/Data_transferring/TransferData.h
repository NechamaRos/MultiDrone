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
<<<<<<< HEAD
    bool sendMessageByChunk(const string& chunk, size_t chunkIndex);
    bool sendMetaData(const Meta_Data& metaData);
    bool sendData(const string& data, const Meta_Data& metaData);
    void sendsAsynchronously(const string& dataAsStr, const Meta_Data& metaData, size_t numChunks, size_t chunk_size, size_t numThreads);
    void sendsSynchronously(const string& dataAsStr, const Meta_Data& metaData);
    void preparingTheDataForTransferring(const string& dataAsStr, const Meta_Data& metaData);
=======
    bool sendMessageByChunk(const string& chunk, size_t chunkIndex, int client_socket);
    bool sendMetaData(const Meta_Data& metaData, int client_socket);
    bool sendData(const string& data, const Meta_Data& metaData, int client_socket);
    void sendsAsynchronously(const string& dataAsStr, const Meta_Data& metaData, size_t numChunks, size_t chunk_size, size_t numThreads, int client_socket);
    void sendsSynchronously(const string& dataAsStr, const Meta_Data& metaData, int client_socket);
    void preparingTheDataForTransferring(const string& dataAsStr, const Meta_Data& metaData, int client_socket);
>>>>>>> 28f3925 (addind my send functions to TransferData.cpp)

    void addChunk(const string& chunk, size_t chunkIndex);
    string getCollectedData();

private:
    map<size_t, string> collectedDataMap;
    mutex dataMutex;
<<<<<<< HEAD
=======
    bool sendMessageByChunk(const string& chunk,int sockfd);
    bool sendMetaData(const Meta_Data& metaData, int client_sockfd);
    bool sendData(const string& data, const Meta_Data& metaData, int client_sockfd);
    void sendsAsynchronously(const string& dataAsStr, const Meta_Data& metaData, size_t numChunks, size_t chunk_size, size_t numThreads, int client_sockfd);
    void sendsSynchronously(const string& dataAsStr, const Meta_Data& metaData, int client_socket);
    void preparingTheDataForTransferring(const string& dataAsStr, const Meta_Data& metaData, int client_sockfd);
>>>>>>> e5026c1 (adding client_sockfd argument to ruti's functions, adding my send() function to ruti's functions)
};
=======
};
>>>>>>> 28f3925 (addind my send functions to TransferData.cpp)
