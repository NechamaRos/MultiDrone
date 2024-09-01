#pragma once
#include "Queue.h"
#include "Message.h"
#include <list>
#include <iostream>

/**
 * @brief Function to check if all queues are empty and all weights are zero.
 *
 * @param listOfTaskQueues Reference to the list of QueueManagement objects.
 * @return true If all queues are empty and all weights are zero.
 * @return false If any queue is not empty or any weight is not zero.
 */
bool CheckQueueIsEmptyAndWeightZeroForAll(std::list<Queue>& listOfTaskQueues) {
	for (auto& qm : listOfTaskQueues) {
		if (!qm.isEmpty() || qm.GetCurrentWeight() > 0)
			return false;
	}
	return true;
}


/**
 * @brief Function to reset the current weights of the queues to their initial values.
 *
 * @param listOfTaskQueues Pointer to the list of QueueManagement objects.
 */
void InitialWeights(std::list<Queue>* listOfTaskQueues) {
	for (auto& queue : *listOfTaskQueues) {
		queue.SetCurrentWeight(queue.GetWeight());
	}
}

/**
 * @brief Function to add additional tasks to non-empty queues.
 *
 * @param listOfTaskQueues Reference to the list of QueueManagement objects.
 */
void addAdditionalTasks(std::list<Queue>& listOfTaskQueues) {
	for (auto& queue : listOfTaskQueues) {
		Message additionalTask = Message();  // יצירת משימה חדשה עם מצביע
		queue.push(additionalTask);  // הוספת המשימה לתור
	}
}

/**
 * @brief Function to perform weighted round robin scheduling.
 *
 * @param listOfTaskQueues Pointer to the list of QueueManagement objects.
 * @param addTasks Boolean flag to indicate whether to add additional tasks during execution.
 */
void WeightedRoundRobin(std::list<Queue>* listOfTaskQueues, bool addTasks) {
	while (true) {
		// Loop until all queues are empty
		while (CheckQueueIsEmptyAndWeightZeroForAll(*listOfTaskQueues) == false) {
			bool allWeightsZero = true;
			bool allQueuesEmpty = true;
			// Iterate through each QueueManagement object in the list
			for (auto& queueManagement : *listOfTaskQueues) {
				// Check if the current queue has remaining weight and if the current queue is not empty
				if (!queueManagement.isEmpty() && queueManagement.GetCurrentWeight() > 0) {
					// Print a message indicating a task is running
					std::cout << "Running task: " << std::endl;
					// Handle the task at the front of the queue
					queueManagement.Handler();
					// Decrease the weight of the current queue
					queueManagement.DecreaseWeight();
				}
				if (queueManagement.GetCurrentWeight() > 0)
					allWeightsZero = false;
				if (!queueManagement.isEmpty())
					allQueuesEmpty = false;
			}
			if (allQueuesEmpty)
				break;
			if (allWeightsZero) {
				InitialWeights(listOfTaskQueues);
			}
			if (addTasks) {
				addAdditionalTasks(*listOfTaskQueues); // הוספת משימות נוספות
			}
		}
	}
}



