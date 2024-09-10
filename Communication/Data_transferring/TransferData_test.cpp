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
using namespace std;
VecValidation vv4({ 1,0 });
Meta_Data<1> mdv4(&vv4);
VecMessage vm14({ 'a','b','c' });
Data<1> vd4(mdv4, &vm14);

MatValidation mv4({ 1 });
Meta_Data<2> mdm4(&mv4);
MatMessage mm14({ {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } });
Data<2> md4(mdm4, &mm14);

D3Validation d3v4({ 1 });
Meta_Data<3> mdd4(&d3v4);
D3Message d3m14({ { {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } } ,{ {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } } });
Data<3> d3d4(mdd4, &d3m14);

int num_drone=0;
TransferData td;

TEST_CASE("TransferData::num_cores function") {
	SUBCASE("Check number of cores") {
		int cores = td.num_cores();
		CHECK(cores > 0);
	}
}

TEST_CASE("TransferData::waiting function") {
    vector<future<bool>> futures;

	SUBCASE("Wait for futures to complete") {
		futures.push_back(async(launch::async, [] { return true; }));
		futures.push_back(async(launch::async, [] { return false; }));

		// Redirect stdout to capture output
		stringstream buffer;
		streambuf* old = cout.rdbuf(buffer.rdbuf());

		td.waiting(futures);

		string output = buffer.str();
		cout.rdbuf(old);

		CHECK(output.find("Result: true") != string::npos);
		CHECK(output.find("Result: false") != string::npos);
	}
}

TEST_CASE("TransferData::sendMessageByChunk function") {
    string chunk = "Test chunk";

    SUBCASE("Successful send") {
        CHECK(td.sendMessageByChunk(chunk, 0, num_drone) == true);
    }

    SUBCASE("Send with exception") {
        CHECK_THROWS_AS(td.sendMessageByChunk("", 0, num_drone), exception);
    }
}

TEST_CASE("TransferData::sendMetaData function") {
    Meta_Data<1> metaData(&vv4);

    SUBCASE("Successful send") {
        CHECK(td.sendMetaData(metaData, num_drone) == true);
    }
	SUBCASE("Successful send") {
		CHECK(td.sendMetaData(metaData, num_drone) == true);
	}

}


TEST_CASE("TransferData::sendData function") {
    string data = "Test data";
    Meta_Data<1> metaData(&vv4);

    SUBCASE("Successful send") {
        CHECK(td.sendData(data, metaData, num_drone) == true);
    }

}

TEST_CASE("TransferData::sendsAsynchronously function") {
	string data = "Test data for async send";
	Meta_Data<1> metaData(&vv4);
	size_t numChunks = 3;
	size_t chunk_size = 4;
	size_t numThreads = 2;

	SUBCASE("Successful asynchronous send") {
		CHECK_NOTHROW(td.sendsAsynchronously(data, metaData, numChunks, chunk_size, numThreads, num_drone));
	}

	SUBCASE("Asynchronous send with failure") {
		// Simulate failure by sending empty data
		string failingData = "";

		CHECK_THROWS_AS(td.sendsAsynchronously(failingData, metaData, numChunks, chunk_size, numThreads, num_drone), runtime_error);
	}
}
TEST_CASE("TransferData::sendsSynchronously function") {
    string data = "Test data for sync send";
    Meta_Data<1> metaData(&vv4);

    SUBCASE("Successful synchronous send") {
        CHECK_NOTHROW(td.sendsSynchronously(data, metaData, num_drone));
    }

    SUBCASE("Synchronous send with exception") {
        CHECK_THROWS_AS(td.sendsSynchronously("", metaData, num_drone), runtime_error);
    }
}

TEST_CASE("TransferData::preparingTheDataForTransferring function") {
	string data = "Test data for transfer";
	Meta_Data<1> metaData(&vv4);

	SUBCASE("Preparing data for synchronous transfer") {
		// Redirect cin for testing
		istringstream input("1");
		cin.rdbuf(input.rdbuf());

		SUBCASE("Preparing data for synchronous transfer") {
			// Redirect cin for testing
			istringstream input("1");
			cin.rdbuf(input.rdbuf());

			CHECK_NOTHROW(td.preparingTheDataForTransferring(data, metaData, num_drone));
		}

		SUBCASE("Preparing data for asynchronous transfer") {
			// Redirect cin for testing
			istringstream input("2");
			cin.rdbuf(input.rdbuf());

			CHECK_NOTHROW(td.preparingTheDataForTransferring(data, metaData, num_drone));
		}
	}
}
TEST_CASE("TransferData::addChunk and getCollectedData functions") {

	SUBCASE("Add and collect chunks") {
		td.addChunk("Chunk1", 0);
		td.addChunk("Chunk2", 1);
		td.addChunk("Chunk3", 2);

		string collectedData = td.getCollectedData();
		CHECK(collectedData == "Chunk1Chunk2Chunk3");
	}

	SUBCASE("Collect data with out-of-order chunks") {
		td.addChunk("Chunk2", 1);
		td.addChunk("Chunk1", 0);
		td.addChunk("Chunk3", 2);

		string collectedData = td.getCollectedData();
		CHECK(collectedData == "Chunk1Chunk2Chunk3");
	}
	td.droneCommunicationManager->infinite_checking_for_incoming_messages();
}

