#pragma once
#define TEST_MODE 0
#include <vector>
#include <map>
#include "slaveEventManager.h"
#include "masterEventManager.h"

using namespace std;

#ifdef TEST_MODE

class MockDataManager {
public:
     MasterEventManager masterManager;
     map<int, SlaveEventManager*> slaveManagers;
     map<int, vector<Message*>> receivedMessages;
     string sortedListFileName;
};

extern MockDataManager mockDataManager;

void mockSendMessage(Message* message);
void mockReceiveMessages(Message* message);
#endif

