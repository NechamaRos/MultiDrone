#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "Message.h"
#include "Queue.h" // Include your Queue class definition
#include "Priority.h"
#include <map>
using namespace std;


Queue* queues[];
map<Priority, int> locations;

// Function to read data from the port (stub)
void* readData() {
	// Your implementation to read data from the port
	Message* message = new Message();
	// Fill in the message data
	return message;
}

void init(Queue* qus) {
	for (int i = 0; i < sizeof(qus); i++) {
		Priority p = qus[i].GetPriority();
		queues[i] = new Queue(qus[i]);
		locations.insert(make_pair(p, i));
	}
}
// Array of queues for different priority levels

// Adjust this based on the number of priority levels you have

void listenToPort(int port) {
	while (true) {
		// Read data from the port
		Message* message = (Message*)readData();

		// Determine the priority of the message ?
		Priority priority = message->priority;
		int pr = getPriorityIndex(priority);
		// Ensure priority is within valid range
		if (pr==-1) {
			std::cerr << "Invalid priority: " << pr << std::endl;
			continue;
		}

		// Push the message into the appropriate queue based on priority
		queues[locations[priority]]->push(*message);
	}
}

int main() {
	int port = 8080; // Replace with the actual port number you are listening to
	std::thread listenerThread(listenToPort, port);
	
	// Join the listener thread to the main thread (or handle as needed)
	listenerThread.join();

	return 0;
}