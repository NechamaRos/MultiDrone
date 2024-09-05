#include "dynamicBuffer.h"

void DynamicBuffer::writeToFile()
{
}

DynamicBuffer::DynamicBuffer(int size)
	: maxSize(size), bufferIndex(0), altBufferIndex(1), min_time(system_clock::now()), max_time(system_clock::now())
{
	buffers[0].reserve(maxSize);
	buffers[1].reserve(maxSize);
}

DynamicBuffer::DynamicBuffer(const DynamicBuffer& other)
	:maxSize(other.maxSize), bufferIndex(other.bufferIndex),
	altBufferIndex(other.altBufferIndex),min_time(other.min_time),
	max_time(other.max_time)
{
	buffers[0] = other.buffers[0];
	buffers[1] = other.buffers[1];

}

DynamicBuffer& DynamicBuffer::operator=(const DynamicBuffer& other)
{
	if (this != &other)
	{
		buffers[0] = other.buffers[0];
		buffers[1] = other.buffers[1];
		maxSize = other.maxSize;
		bufferIndex = other.bufferIndex;
		altBufferIndex = other.altBufferIndex;
		min_time = other.min_time;
		max_time = other.max_time;
	}
	return *this;
}



int DynamicBuffer::getBufferIndex()
{
	return bufferIndex;
}


/// <summary>
/// get event and add it to the buffer
/// </summary>
/// <param name="event"></param>
/// <returns>true if the current buffer is full and false else</returns>
bool DynamicBuffer::addEvent(const Event& event)
{
	bool isFull = false;
	unique_lock<mutex> lock(mutex_);

	if (buffers[bufferIndex].size() == maxSize) 
	{
		isFull = true;
		swap(bufferIndex, altBufferIndex);
	}

	buffers[bufferIndex].push_back(event);
	return isFull;
}

vector<Event>& DynamicBuffer::getBuffer() 
{
	unique_lock<mutex> lock(mutex_);
	return buffers[bufferIndex];
}

vector<Event>& DynamicBuffer::getAltBuffer()
{
	return buffers[altBufferIndex];
}



void DynamicBuffer::clearAltBuffer()
{
	unique_lock<mutex> lock(mutex_);
	buffers[altBufferIndex].clear();
}
