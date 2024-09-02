#include "mockDataManager.h"

MockDataManager mockDataManager;

void mockSendMessage(Message* message) {
    auto it = mockDataManager.slaveManagers.find(message->droneId);
    if (it != mockDataManager.slaveManagers.end()) {
        SlaveEventManager* slave = it->second;
        slave->getEvents(message);
    }
}

void mockReceiveMessages(Message* message) {
    mockDataManager.masterManager.receiveEventsFromSlave(message);
}
