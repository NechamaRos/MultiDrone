#pragma once
#include "MessageParams.h"
#include "Priority.h"
struct Message
{
	int messageId;
	int reqId;
	MessageType messageType;
	int droneId;
	MessageParams* messageParams;
	Priority priority;

	Message() :messageId(counter++), messageParams(nullptr) {}

	// Copy constructor
	/*Message(const Message& message)
		: messageId(counter++),reqId(message.reqId),messageType(message.messageType),droneId(message.droneId),
		messageParams(message.messageParams),priority(message.priority)
	{}*/

	Message& operator=(const Message& other)
	{
		if (this != &other)
		{
			// Note: messageId should remain unique, so we do not copy it.
			reqId = other.reqId;
			messageType = other.messageType;
			droneId = other.droneId;
			messageParams = other.messageParams; // Assuming proper copy behavior in MessageParams
			priority = other.priority;
		}
		return *this;
	}

private:
	int static counter;

};

