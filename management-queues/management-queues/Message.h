#pragma once
#include "Priority.h"
#include "MessageType.h"
#include "MessageParams.h"
//#include "dataOfMessage.h"

struct Message
{
    int messageId;
    int reqId;
    MessageType messageType;
    int droneId;
    MessageParams* messageParams;

    Message() :messageId(counter++) {}
    // for testing
    Message(MessageType mt) :messageId(counter++),messageType(mt) {}

    Message& operator=(const Message& other);

    Priority GetPriority();

private:
    int static counter;
};