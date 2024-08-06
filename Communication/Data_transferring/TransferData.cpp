#include <iostream>     
#include <future>      
#include <thread>
#include <chrono>
#include <vector>
#include <bitset>
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
         cout << "Result: " <<  boolalpha << result <<  endl;
    }
}
bool TransferData::sendMessageByChunk(const string& chunk)
{
    try
    {
        if (chunk.empty()) {
            throw  runtime_error("Chunk is empty");
        }
        //Noa function, open the socket.
         this_thread::sleep_for( chrono::milliseconds(100));
         cout << "Sent chunk: " << chunk <<  endl;
        return true;
    }
    catch (const  exception&)
    {
        throw exception("The send failed");
    }

}
