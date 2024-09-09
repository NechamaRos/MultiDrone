#include "Message.h"
#include "dataOfMessage.h"
#include <cstddef>

int Message::counter = 0;

Priority Message::GetPriority() {
	return dataOfMessageVector[static_cast<size_t>(messageType)].priority;
}

Message& Message:: operator=(const Message& other)
{
    if (this != &other)
    {
        // Note: messageId should remain unique, so we do not copy it.
        reqId = other.reqId;
        messageType = other.messageType;
        droneId = other.droneId;
        messageParams = other.messageParams; // Assuming proper copy behavior in MessageParams
        //priority = other.priority;
    }
    return *this;
}
