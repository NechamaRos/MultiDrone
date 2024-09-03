#pragma once
#include "MessageType.h"
#include "Priority.h"
#include "Message.h"
struct dataOfMessage {
    Priority priority;
    void (*func)(Message* message);
};

//functions
void getSortedEventListFromSlaves(Message* message);
void getEvents(Message* message);



dataOfMessage dataOfMessageVector[int(MessageType::size)];

//initial vector pointer to function
void InitialPointerToFunction() {
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_SEND_EVENTS)].func = &getEvents;
}