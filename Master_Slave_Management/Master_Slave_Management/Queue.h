#pragma once
#include <iostream>
#include <stdexcept>
#include <mutex>
#include "RequestMessage.h"
#include "Priority.h"

// Node structure for the doubly linked list
struct Node {
	Message data;
	Node* next;
	Node* prev;

	Node(Message data) : data(data), next(nullptr), prev(nullptr) {}
};

typedef void (*FuncPtr)();

class Queue {
private:
	std::vector<FuncPtr> pointersToFunctions;

	Node* head;
	Node* tail;
	Priority priority;
	int weight;
	int currentWeight;
	int maxOfMessages;
	int numOfMessages;
	int maxOfMessages;
	std::mutex mtx;

	void clear();
	void copyFrom(const Queue& other);

public:
	Queue(Priority priority = Priority::Low) : head(nullptr), tail(nullptr), priority(priority) {}

	// Destructor to clean up the allocated memory
	~Queue();

	Queue(const Queue& other) : head(nullptr), tail(nullptr), numOfMessages(0) {
		copyFrom(other);
	}

	Queue& operator=(const Queue& other);

	void push(Message data);

	Message pop();

	Node* pick(int index);

	Message top() const;

	int pass() const;

	bool isEmpty() const;

	Priority GetPriority() const;

	void setPriority(Priority newPriority);

	int GetSize();

	void DecreaseWeight();

	int GetCurrentWeight() const;

	void Handler();
};