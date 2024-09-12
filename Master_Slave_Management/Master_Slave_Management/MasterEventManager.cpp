#include "masterEventManager.h"
#include "mockDataManager.h"

int MasterEventManager::reqCounter = 0;

/// <summary>
/// Gets a list of event lists and merges them into one sorted event list.
/// </summary>
/// <param name="allEvents"> list of event lists</param>
void MasterEventManager::mergeAndSortEvents(vector<vector<Event>>& allEvents)
{
	auto compare = [](const pair<Event,size_t>& a, const pair<Event,size_t>& b) {
		return a.first > b.first;
	};
	priority_queue<pair<Event, size_t>, vector<pair<Event, size_t>>, decltype(compare)> minHeap(compare);

	for (size_t i = 0; i < allEvents.size(); i++)
	{
		if (!allEvents[i].empty())
		{
			minHeap.emplace(allEvents[i][0],i);
		}
	}

	vector<size_t> indices(allEvents.size(), 0);
	vector<Event> sortedEvents;

	while (!minHeap.empty())
	{
		auto topElement = minHeap.top();
		Event event = topElement.first;
		size_t listIndex = topElement.second;
		//auto [event, listIndex] = minHeap.top();
		minHeap.pop();

		sortedEvents.push_back(event);

		indices[listIndex]++;
		if (indices[listIndex] < allEvents[listIndex].size())
		{
			minHeap.emplace(allEvents[listIndex][indices[listIndex]],listIndex);
		}
	}
	string fileName = fileHandler.writeToFile(sortedEvents, true);
	cout << "The Sorted List File saves in: " << fileName;

	#ifdef TEST_MODE
	mockDataManager.sortedListFileName = fileName;
	#endif

	//Emptying the event lists of the current request
	currentEventsFromSlaves.allEvents.clear();
}

MasterEventManager::MasterEventManager()
	:SlaveEventManager()
{
}

MasterEventManager::MasterEventManager(const MasterEventManager& other)
	:SlaveEventManager(other)
{
}

MasterEventManager& MasterEventManager::operator=(const MasterEventManager& other)
{
	if (this != &other)
	{
		SlaveEventManager::operator=(other);
	}
	return *this;
}

int MasterEventManager::getNumberOfResFromSlaves()
{
	return currentEventsFromSlaves.numberOfResFromSlaves;
}

/// <summary>
/// Checks if all the drones have already responded,
/// and if so sends the event lists for sorting. 
/// </summary>
void MasterEventManager::areAllSlavesResponded()
{
	if (currentEventsFromSlaves.numberOfReqFromSlaves == currentEventsFromSlaves.numberOfResFromSlaves)
		mergeAndSortEvents(currentEventsFromSlaves.allEvents);
}



/// <summary>
/// Receives the response with the event list from the drone and adds it to the event lists only if it belongs to the current request
/// and check if all the drones have already responded.
/// </summary>
/// <param name="message">The response message received from the drone with the list of events according to the requested range.</param>
void MasterEventManager::receiveEventsFromSlave(Message* message)
{
	vector<Event> eventList = message->messageParams->STM_GET_EVENTS_PARAMS.events;
	int messageReqId = message->reqId;
	if (messageReqId == currentEventsFromSlaves.reqId)
	{
		currentEventsFromSlaves.numberOfResFromSlaves++;
		currentEventsFromSlaves.allEvents.push_back(eventList);
		areAllSlavesResponded();
	}
}



/// <summary>
/// Sends requests for a list of events in a specified time range to multiple drones.
/// </summary>
/// <param name="slaves">List of IDs of drones from which you want the sorted list </param>
/// <param name="startTime">The beginning of the time range of the desired events</param>
/// <param name="endTime">The end of the time range of the desired events</param>
/// <param name="maxSize">The maximum size of the list of events that will be returned</param>


void MasterEventManager::sendReqToEventListFromSlaves(vector<int> slaves, tm& startTime, tm& endTime, size_t maxSize)
{
	int reqId = reqCounter++;
	currentEventsFromSlaves.reqId = reqId;
	vector<vector<Event>> allEvents;
	size_t maxEventPerSlave = maxSize / slaves.size();


	for (int slaveId : slaves)
	{
		currentEventsFromSlaves.numberOfReqFromSlaves++;
		Message* message = new Message();
		message->reqId = reqId;
		message->droneId = slaveId;
		message->messageType = MessageType::MTS_SEND_EVENTS;
		message->messageParams = new MessageParams();
		message->messageParams->MTS_SEND_EVENTS_PARAMS.startTime = startTime;
		message->messageParams->MTS_SEND_EVENTS_PARAMS.endTime = endTime;
		message->messageParams->MTS_SEND_EVENTS_PARAMS.maxSize = maxEventPerSlave;

		sendMessage(message);

		#ifdef TEST_MODE
		mockDataManager.receivedMessages[slaveId].push_back(message);
		#endif 

	}
}


