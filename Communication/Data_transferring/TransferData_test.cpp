#include "../Communication/doctest.h"
#include <vector>
#include <future>
#include <iostream>
#include <sstream>
#include "TransferData.h"
#include "../Validation/VecValidation.h"
#include "../Communication/Meta_Data.h"
#include "../Communication/VecMessage.h"
#include "../Communication/Data.h"

VecValidation vv4({ 1,0 });
Meta_Data mdv4(&vv4);
VecMessage vm14({ 'a','b','c' });
Data vd4(mdv4, &vm14);

MatValidation mv4({ 1 });
Meta_Data mdm4(&mv4);
MatMessage mm14({ {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } });
Data md4(mdm4, &mm14);

D3Validation d3v4({ 1 });
Meta_Data mdd4(&d3v4);
D3Message d3m14({ { {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } } ,{ {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } } });
Data d3d4(mdd4, &d3m14);

int client_socket = 0;

TEST_CASE("TransferData::num_cores function") {
    TransferData td;

    SUBCASE("Check number of cores") {
        int cores = td.num_cores();
        CHECK(cores > 0);
    }
}

TEST_CASE("TransferData::waiting function") {
    TransferData td;
    std::vector<std::future<bool>> futures;

    SUBCASE("Wait for futures to complete") {
        futures.push_back(std::async(std::launch::async, [] { return true; }));
        futures.push_back(std::async(std::launch::async, [] { return false; }));

        // Redirect stdout to capture output
        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

        td.waiting(futures);

        std::string output = buffer.str();
        std::cout.rdbuf(old);

        CHECK(output.find("Result: true") != std::string::npos);
        CHECK(output.find("Result: false") != std::string::npos);
    }
}

TEST_CASE("TransferData::sendMessageByChunk function") {
    TransferData td;
    std::string chunk = "Test chunk";

    SUBCASE("Successful send") {
<<<<<<< HEAD
        CHECK(td.sendMessageByChunk(chunk, 0) == true);
    }

    SUBCASE("Send with exception") {
        CHECK_THROWS_AS(td.sendMessageByChunk("", 0), std::exception);
=======
        CHECK(td.sendMessageByChunk(chunk, client_socket) == true);
    }

    SUBCASE("Send with exception") {
        CHECK_THROWS_AS(td.sendMessageByChunk("", client_socket), std::exception);
>>>>>>> e5026c1 (adding client_sockfd argument to ruti's functions, adding my send() function to ruti's functions)
    }
}

TEST_CASE("TransferData::sendMetaData function") {
    TransferData td;
    Meta_Data metaData(&vv4);

    SUBCASE("Successful send") {
        CHECK(td.sendMetaData(metaData, client_socket) == true);
    }

}

TEST_CASE("TransferData::sendData function") {
    TransferData td;
    std::string data = "Test data";
    Meta_Data metaData(&vv4);

    SUBCASE("Successful send") {
        CHECK(td.sendData(data, metaData, client_socket) == true);
    }

}

TEST_CASE("TransferData::sendsAsynchronously function") {
    TransferData td;
    std::string data = "Test data for async send";
    Meta_Data metaData(&vv4);
    size_t numChunks = 3;
    size_t chunk_size = 4;
    size_t numThreads = 2;

    SUBCASE("Successful asynchronous send") {
        CHECK_NOTHROW(td.sendsAsynchronously(data, metaData, numChunks, chunk_size, numThreads, client_socket));
    }

    SUBCASE("Asynchronous send with failure") {
        // Simulate failure by sending empty data
        std::string failingData = "";

        CHECK_THROWS_AS(td.sendsAsynchronously(failingData, metaData, numChunks, chunk_size, numThreads, client_socket), std::runtime_error);
    }
}

TEST_CASE("TransferData::sendsSynchronously function") {
    TransferData td;
    std::string data = "Test data for sync send";
    Meta_Data metaData(&vv4);

    SUBCASE("Successful synchronous send") {
        CHECK_NOTHROW(td.sendsSynchronously(data, metaData, client_socket));
    }

    SUBCASE("Synchronous send with exception") {
        CHECK_THROWS_AS(td.sendsSynchronously("", metaData, client_socket), std::runtime_error);
    }
}

TEST_CASE("TransferData::preparingTheDataForTransferring function") {
    TransferData td;
    std::string data = "Test data for transfer";
    Meta_Data metaData(&vv4);

    SUBCASE("Preparing data for synchronous transfer") {
        // Redirect cin for testing
        std::istringstream input("1");
        std::cin.rdbuf(input.rdbuf());

        CHECK_NOTHROW(td.preparingTheDataForTransferring(data, metaData, client_socket));
    }

    SUBCASE("Preparing data for asynchronous transfer") {
        // Redirect cin for testing
        std::istringstream input("2");
        std::cin.rdbuf(input.rdbuf());

        CHECK_NOTHROW(td.preparingTheDataForTransferring(data, metaData, client_socket));
    }
}

TEST_CASE("TransferData::addChunk and getCollectedData functions") {
    TransferData td;

    SUBCASE("Add and collect chunks") {
        td.addChunk("Chunk1", 0);
        td.addChunk("Chunk2", 1);
        td.addChunk("Chunk3", 2);

        std::string collectedData = td.getCollectedData();
        CHECK(collectedData == "Chunk1Chunk2Chunk3");
    }

    SUBCASE("Collect data with out-of-order chunks") {
        td.addChunk("Chunk2", 1);
        td.addChunk("Chunk1", 0);
        td.addChunk("Chunk3", 2);

        std::string collectedData = td.getCollectedData();
        CHECK(collectedData == "Chunk1Chunk2Chunk3");
    }
}
