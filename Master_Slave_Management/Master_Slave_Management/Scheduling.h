#pragma once
#include "Queue.h"
#include "Message.h"
#include <list>
#include <iostream>

// Function to check if all queues are empty
bool CheckQueueIsEmptyAndWeightZeroForAll(std::list<Queue>& listOfTaskQueues);
void InitialWeights(std::list<Queue>* listOfTaskQueues);
void WeightedRoundRobin(std::list<Queue>* listOfTaskQueues, bool addTasks);
void addAdditionalTasks(std::list<Queue>& listOfTaskQueues);
