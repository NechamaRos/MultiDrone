#pragma once
#include <algorithm>
#include <map>
#include <queue>
#include "slaveEventManager.h"
#include "Message.h"
//#include <CL/sycl.hpp>
//using namespace sycl;
using namespace std;

struct CurrentEventsFromSlaves
{
	int reqId;
	vector<vector<Event>> allEvents;
	int numberOfReqFromSlaves = 0;
	int numberOfResFromSlaves = 0;
};

class MasterEventManager: virtual public SlaveEventManager
{
    static int reqCounter;
	CurrentEventsFromSlaves currenteventsFromSlaves;
	void mergeAndSortEvents(vector<vector<Event>>& allEvents);
	void areAllSlavesResponded();
public:
	MasterEventManager();
	MasterEventManager(const MasterEventManager& other);
	MasterEventManager& operator=(const MasterEventManager& other);
	int getNumberOfResFromSlaves();
	void receiveEventsFromSlave(Message* message);
	void sendReqToEventListFromSlaves(vector<int> slaveIds, tm& startTime, tm& endTime, size_t maxSizeInBytes);
};

