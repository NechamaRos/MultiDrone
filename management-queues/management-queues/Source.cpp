#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "Message.h"
#include "Queue.h" 
#include "Priority.h"
#include <map>
using namespace std;

// Array of queues for different priority levels
Queue* queues[5];
map<Priority, int> keysForPriorities;

int getPriorityIndex(Priority priority) {
	switch (priority) {
	case Priority::Low:        return 0;
	case Priority::MediumLow:  return 1;
	case Priority::Medium:     return 2;
	case Priority::MediumHigh: return 3;
	case Priority::High:       return 4;
	default:                   return -1;
	}
}

void* readData() {
	//  implementation to read data from the port
	Message* message = new Message(static_cast<MessageType>(rand() % 15));
	return message;
}

void init(Queue* qus, int size) { // Pass the size of the array as an argument
	for (int i = 0; i < size; i++) {
		Priority p = qus[i].GetPriority();
		queues[i] = new Queue(qus[i]);
		keysForPriorities.insert(make_pair(p, i));
	}
}

void listenToPort(int port) {
	while (true) {
		// Read data from the port
		Message* message = (Message*)readData();

		// Determine the priority of the message 
		Priority priority = message->GetPriority();
		int pr = getPriorityIndex(priority);
		// Ensure priority is within valid range
		if (pr == -1) {
			std::cerr << "Invalid priority: " << pr << std::endl;
			continue;
		}

		// Push the message into the appropriate queue based on priority
		queues[keysForPriorities[priority]]->push(*message);
	}
}

int main() {
	int port = 8080; // Replace with the actual port number you are listening to
	thread listenerThread(listenToPort, port);

	// Join the listener thread to the main thread (or handle as needed)
	listenerThread.join();

	return 0;
}
