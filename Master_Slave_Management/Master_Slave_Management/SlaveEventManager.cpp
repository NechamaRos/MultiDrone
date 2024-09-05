#include "slaveEventManager.h"
#include "mockDataManager.h"

int SlaveEventManager::counter = 0;

void SlaveEventManager::sendMessage(Message* message)
{
}

int SlaveEventManager::getSlaveId()
{
	return id;
}

SlaveEventManager::SlaveEventManager()
	:id(counter++),buffer(30),fileHandler("baseFile_",10)
{
}

/// <summary>
/// create a new slave 
/// </summary>
/// <param name="bufferSize">The number of events that will be saved in the dynamic buffer before being written to the file</param>
/// <param name="baseFilename">The base of the name of the files where the slave's events will be saved</param>
/// <param name="maxFiles">The maximum number of files that will be saved.</param>
SlaveEventManager::SlaveEventManager(int bufferSize, string baseFilename, size_t maxFiles):
	id(counter++), buffer(bufferSize), fileHandler(baseFilename, maxFiles)
{
}

SlaveEventManager::SlaveEventManager(const SlaveEventManager& other)
	:id(other.id), buffer(other.buffer), fileHandler(other.fileHandler)
{
}

SlaveEventManager& SlaveEventManager::operator=(const SlaveEventManager& other)
{
	if (this != &other)
	{
		id = other.id;
		buffer = other.buffer;
		fileHandler = other.fileHandler;
	}
	return *this;
}

/// <summary>
/// adds a new event
/// </summary>
/// <param name="event">message of event</param>
void SlaveEventManager::addEvent(string event)
{
	addEvent(Event(event));
}

/// <summary>
/// Adds a new event to the buffer. 
/// If the buffer is full, write it to a file.
/// </summary>
/// <param name="event">event to add</param>
void SlaveEventManager::addEvent(const Event& event)
{
	if (buffer.addEvent(event))
	{
		fileHandler.writeToFile(buffer.getAltBuffer());
		buffer.clearAltBuffer();
	}
}

/// <summary>
/// API that receives the request to get a list of events from the master. 
/// </summary>
/// <param name="message">the message with the range time and maximum size for the event list</param>
void SlaveEventManager::getEvents(Message* message)
{
	int reqId = message->reqId;
	tm startTime = message->messageParams->MTS_SEND_EVENTS_PARAMS.startTime;
	tm endTime = message->messageParams->MTS_SEND_EVENTS_PARAMS.endTime;
	size_t maxSize = message->messageParams->MTS_SEND_EVENTS_PARAMS.maxSize;

	readEvents(reqId,startTime,endTime,maxSize);	
}



/// <summary>
/// Adds to the 'events' vector the events in the buffer in the range 
/// </summary>
/// <param name="events">An empty list to which the events matching the condition are added</param>
/// <param name="startTime">The beginning of the time range of the desired events</param>
/// <param name="endTime">The end of the time range of the desired events</param>
/// <param name="maxSize">The maximum size of the list of events that will be sended</param>
void SlaveEventManager::addBufferEventsToVector(vector<Event>& events, const tm startTime, const tm endTime, size_t& maxSize)
{
	Event event;
	tm eventTime;
	vector<Event> bufferEvents = buffer.getBuffer();
	size_t currentSize = 0;

	for (auto it = bufferEvents.rbegin(); it != bufferEvents.rend(); ++it) {
		event = *it;
		eventTime = event.getTime();
		if(event >= startTime && event<=endTime)
		{
			size_t eventSize = event.getSize();
			if (currentSize + eventSize > maxSize)
			{
				break; 
			}
			events.push_back(event);
			currentSize += eventSize;
		}
	}
	maxSize -= currentSize;
}


/// <summary>
/// send a response with the events list to  the master
/// </summary>
/// <param name="reqId">the global request ID of the master received with the request.</param>
/// <param name="events">The list of events was read from the drone's memory according to the time range.</param>

void SlaveEventManager::sendResponseToMaster(int reqId, vector<Event> events)
{
	Message* message = new Message();
	message->reqId = reqId;
	message->droneId = this->id;
	message->messageType = MessageType::STM_GET_EVENTS;

	message->messageParams = new MessageParams();
	new (&message->messageParams->STM_GET_EVENTS_PARAMS) STM_GET_EVENTS_PARAMS();

	message->messageParams->STM_GET_EVENTS_PARAMS.events.insert(
		message->messageParams->STM_GET_EVENTS_PARAMS.events.end(),
		events.begin(),
		events.end()
	);
	sendMessage(message);

	#ifdef TEST_MODE
		mockReceiveMessages(message);
	#endif 


	message->messageParams->STM_GET_EVENTS_PARAMS.~STM_GET_EVENTS_PARAMS();

	// Deleting the message
	delete message;
}



/// <summary>
/// Reads and returns as a response a list of events according to the received range and size. 
/// First reads the events in the buffer (only if they are in the range) 
/// and then reads from the files according to the remaining size.
/// </summary>
/// <param name="reqId"> The master's general request ID</param>
/// <param name="startTime">The beginning of the time range of the desired events</param>
/// <param name="endTime">The end of the time range of the desired events</param>
/// <param name="maxSize">The maximum size of the list of events that will be sended</param>
void SlaveEventManager::readEvents(int reqId, const tm startTime, const tm endTime, size_t maxSize)
{
	vector<Event> events;
	Event firstEvent = buffer.getBuffer().front();
	Event lastEvent = buffer.getBuffer().back(); 
	if(lastEvent>=startTime && firstEvent<=endTime) //Checks if there are events in the buffer that match the time range
	{
		addBufferEventsToVector(events, startTime, endTime, maxSize);// Adds the events in the buffer and reduces maxSize accordingly.
	}

	//Only if we have not reached the maximum size and it is also possible that there will be events on the disk that fit the range -
	//we will read them from the disk and add them to the vector
	if(maxSize>0 && firstEvent>startTime) 
		fileHandler.readFromDiskAndAddToEvents(events, startTime, endTime, maxSize);

	reverse(events.begin(), events.end()); 

	//Sends the response to the master
	sendResponseToMaster(reqId, events);

}
