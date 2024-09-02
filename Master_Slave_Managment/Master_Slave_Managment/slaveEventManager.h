#pragma once
#include "dynamicBuffer.h"
#include "fileHandler.h"
#include "Message.h"
#include <vector>

class SlaveEventManager
{
	int id;

	static int counter;
	void addBufferEventsToVector(vector<Event>& events, const tm startTime, const tm endTime, size_t& maxSizeInBytes);
	void sendResponseToMaster(int reqId, vector<Event> events);

protected:
	DynamicBuffer buffer;
	FileHandler fileHandler;

	void sendMessage(Message* message);

public:
	int getSlaveId();
	SlaveEventManager();
	SlaveEventManager(int bufferSize, string baseFilename, size_t maxFiles);
	SlaveEventManager(const SlaveEventManager& other);
	SlaveEventManager& operator= (const SlaveEventManager& other);
	void addEvent(string event);
	void addEvent(const Event& event);
	void getEvents(Message* message);
	void readEvents(int reqId, const tm startTime, const tm endTime, size_t maxSizeInBytes);
};

