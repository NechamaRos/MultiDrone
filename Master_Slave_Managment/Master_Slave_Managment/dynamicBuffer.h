#pragma once
#include "event.h"
#include <mutex>
#include <vector>
using namespace std;

class DynamicBuffer
{
	vector<Event> buffers[2];
	int maxSize;
	int bufferIndex;
	int altBufferIndex = 1;
	time_point<system_clock> min_time;
	time_point<system_clock> max_time;
	mutex mutex_;


	void writeToFile();

public:
	DynamicBuffer(int size);
	DynamicBuffer(const DynamicBuffer& other);
	DynamicBuffer& operator=(const DynamicBuffer& other);
	int getBufferIndex();
	bool addEvent(const Event& event);
	vector<Event>& getBuffer();
	vector<Event>& getAltBuffer();
	void clearAltBuffer();
};

