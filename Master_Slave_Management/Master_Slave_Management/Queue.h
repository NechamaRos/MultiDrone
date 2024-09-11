#pragma once
#include <iostream>
#include <stdexcept>
#include <mutex>
#include <vector>
#include "Message.h"

// Node structure for the doubly linked list
struct Node {
	Message* data;
	Node* next;
	Node* prev;

	Node(Message* data) : data(data), next(nullptr), prev(nullptr) {}
};
typedef void (*FuncPtr)();

class Queue {
private:
	//
	std::vector<FuncPtr> pointersToFunctions;

	//
	Node* head;
	Node* tail;
	int priority;
	int weight;
	int currentWeight;
	std::mutex mtx;
public:
	// Constructor to initialize an empty queue with a default priority
	Queue(int weight, int priority = 0) : head(nullptr), tail(nullptr), priority(priority), weight(weight), currentWeight(weight) {}

	// Destructor to clean up the allocated memory
	~Queue() {
		while (head != nullptr) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
	}

	// Push an item to the end of the queue
	void push(Message data);

	// Pop an item from the front of the queue
	Message* pop();

	// Pick (remove) an item from the middle of the queue
	Node* pick(int index);

	Message* top() {
		return head->data;
	}
	// Pass (iterate) over the queue
	int pass() const;

	// Check if the queue is empty
	bool isEmpty() const;

	// Get the current priority of the queue
	int getPriority() const;

	// Set a new priority for the queue
	void setPriority(int newPriority);

	int GetSize() {
		return pass();
	}

	//Decrease the weight by one
	void DecreaseWeight() {
		this->currentWeight -= 1;
	}
	int GetCurrentWeight() const {
		return currentWeight;
	}
	void SetCurrentWeight(int w) { currentWeight = w; }
	int GetWeight() const {
		return weight;
	}
	//Handler the message
	void Handler();


};

