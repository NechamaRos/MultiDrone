#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include "doctest.h"
#include "masterEventManager.h"
#include "slaveEventManager.h"
#include "mockDataManager.h"
using namespace std;

string removeSpaces(const string& str) {
	string result;
	for (char c : str) {
		if (c != ' '&& c != '\n') {
			result += c;
		}
	}
	return result;
}

bool checkFileContent(const string& filename, const vector<string>& expectedLines) {
	ifstream file(filename);
	REQUIRE(file.is_open());

	vector<string> fileLines;
	string line;
	while (getline(file, line)) {
		fileLines.push_back(removeSpaces(line));
	}
	file.close();

	// Checking the compatibility of the number of lines
	if (fileLines.size() != expectedLines.size())
		return false;

	// Comparing the rows one by one
	for (size_t i = 0; i < expectedLines.size(); ++i) {
		if (fileLines[i] != expectedLines[i])
			return false;
	}
	return true;
}

TEST_CASE("Master-Slave Communication Test") {
	#undef TEST_MODE
	#define TEST_MODE 1

	time_point<system_clock> now = system_clock::now();


	time_t now_time_t = system_clock::to_time_t(now);
	tm* now_tm = localtime(&now_time_t);
	tm now_tm_copy = *now_tm;

	if (now_tm != nullptr) {
		cout << "Current time: " << put_time(now_tm, "%d-%m-%Y_%H.%M.%S") << endl;
	}
	Event e1(*now_tm, "event1");

	now_tm->tm_sec += 2;
	Event e2(*now_tm, "event2");

	now_tm->tm_sec += 2;
	Event e3(*now_tm, "event3");


	now_tm->tm_sec += 1;
	Event e4(*now_tm, "event4");

	now_tm->tm_sec += 1;
	Event e5(*now_tm, "event5");

	now_tm->tm_sec += 1;
	Event e6(*now_tm, "event6");

	now_tm->tm_sec += 1;
	Event e7(*now_tm, "event7");

	now_tm->tm_sec += 1;
	Event e8(*now_tm, "event8");

	now_tm->tm_sec += 1;
	Event e9(*now_tm, "event9");

	now_tm->tm_sec += 2;
	Event e10(*now_tm, "event10");

	now_tm->tm_sec += 2;
	Event e11(*now_tm, "event11");

	time_t now_seconds = mktime(&now_tm_copy);
	now_seconds += 120;
	
	vector<Event> events = {
		e2,e3,e4,e5,e6,e7,e8,e9,e10,e11
	};

	//הגדרת הרחפן הראשון והוספת אירועים 
	SlaveEventManager slave1(3, "SEM1_file", 2);
	slave1.addEvent(e1);
	slave1.addEvent(e2);
	slave1.addEvent(e5);
	slave1.addEvent(e6);
	slave1.addEvent(e7);
	slave1.addEvent(e8);

	//הגדרת הרחפן השני והוספת אירועים
	SlaveEventManager slave2(2, "SEM2_file", 2);
	slave2.addEvent(e3);
	slave2.addEvent(e4);
	slave2.addEvent(e9);
	slave2.addEvent(e10);
	slave2.addEvent(e11);

    MasterEventManager master;

    mockDataManager.slaveManagers[1] = &slave1;
    mockDataManager.slaveManagers[2] = &slave2;

	mockDataManager.masterManager = master;


	vector<int> slaveIds = { 1,2 };
	tm startTime = now_tm_copy;
	tm endTime = *(localtime(&(now_seconds)));
	size_t sizeInBytes = 900;

	mockDataManager.masterManager.sendReqToEventListFromSlaves(slaveIds, startTime, endTime, sizeInBytes);

    for (auto& [slaveId, messages] : mockDataManager.receivedMessages) {
        for (Message* message: messages)
        {
            mockSendMessage(message);
        }
    }

	vector<string> eventsInString;
	for (Event event : events) 
	{
		eventsInString.push_back(removeSpaces(event.getFormatTime() + "  " + event.getMessage()));
	}
	bool isContentFileTrue = checkFileContent(mockDataManager.sortedListFileName, eventsInString);

	CHECK(isContentFileTrue==true);

}
