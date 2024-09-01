#pragma once
#include "Queue.h"
#include <iostream>
#include <stdexcept>
#include <mutex>
#include "Message.h"
#include "dataOfMessage.h"


// Push an item to the end of the queue
void Queue::push(Message data) {
	std::lock_guard<std::mutex> lock(mtx);
	Node* newNode = new Node(&data);
	if (tail == nullptr) {
		head = tail = newNode;
	}
	else {
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}
}

// Pop an item from the front of the queue
Message* Queue::pop() {
	std::lock_guard<std::mutex> lock(mtx);
	if (head == nullptr) {
		throw std::out_of_range("Queue is empty");
	}
	Node* temp = head;
	head = head->next;
	if (head != nullptr) {
		head->prev = nullptr;
	}
	else {
		tail = nullptr;
	}
	return temp->data;
}

// Pick (remove) an item from the middle of the queue
Node* Queue::pick(int index) {
	std::lock_guard<std::mutex> lock(mtx);
	if (head == nullptr) {
		throw std::out_of_range("Queue is empty");
	}
	Node* temp = head;
	for (int i = 0; i < index && temp != nullptr; ++i) {
		temp = temp->next;
	}
	if (temp == nullptr) {
		throw std::out_of_range("Index out of range");
	}
	if (temp->prev != nullptr) {
		temp->prev->next = temp->next;
	}
	else {
		head = temp->next;
	}
	if (temp->next != nullptr) {
		temp->next->prev = temp->prev;
	}
	else {
		tail = temp->prev;
	}
	return temp;
}

// Pass (iterate) over the queue
int Queue::pass() const {
	int counter = 0;
	Node* current = head;
	while (current != nullptr) {
		counter++;
		//std::cout << current->data.getMessage();
		current = current->next;
	}
	//std::cout << std::endl;
	return counter;
}

// Check if the queue is empty
bool Queue::isEmpty() const {
	return head == nullptr;
}

// Get the current priority of the queue
int Queue::getPriority() const {
	return priority;
}

// Set a new priority for the queue
void Queue::setPriority(int newPriority) {
	priority = newPriority;
}

//
void Queue::Handler() {
	Message* message = pop();
	//Handles the message in the appropriate function	
	dataOfMessageVector[static_cast<size_t>(message->messageType)].func(message);
}


