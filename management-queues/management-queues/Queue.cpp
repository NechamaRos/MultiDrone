#include "Queue.h"
#include <iostream>
#include <stdexcept>
#include <mutex>
using namespace std;

Queue::~Queue() {
	while (head != nullptr) {
		Node* temp = head;
		head = head->next;
		delete temp;
	}
}
void Queue::clear() {
	while (head != nullptr) {
		Node* temp = head;
		head = head->next;
		delete temp;
	}
	tail = nullptr;
	numOfMessages = 0;
}


void Queue::push(Message data) {
	if (numOfMessages == maxOfMessages) {
		cerr << "אין מקום יותר";
	}
	lock_guard<mutex> lock(mtx);
	//SlaveEventManager::addEvent("ID::push")
	Node* newNode = new Node(data);
	if (tail == nullptr) {
		head = tail = newNode;
	}
	else {
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}
	numOfMessages++;
}

Message Queue::pop() {
	lock_guard<mutex> lock(mtx);
	//  SlaveEventManager::addEvent("ID::pop")
	if (head == nullptr) {
		throw out_of_range("Queue is empty");
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
	numOfMessages--;
}

Node* Queue::pick(int index) {
	//SlaveEventManager::addEvent("ID::pick")
	lock_guard<mutex> lock(mtx);
	if (head == nullptr) {
		throw out_of_range("Queue is empty");
	}

	Node* temp = head;
	for (int i = 0; i < index && temp != nullptr; ++i) {
		temp = temp->next;
	}

	if (temp == nullptr) {
		throw out_of_range("Index out of range");
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

Message Queue::top() const {
	return head->data;
}

int Queue::pass() const {
	int counter = 0;
	Node* current = head;
	while (current != nullptr) {
		counter++;
		//cout << current->data.getMessage();
		current = current->next;
	}
	//cout << endl;
	return counter;
}

bool Queue::isEmpty() const {
	return head == nullptr;
}

Priority Queue::GetPriority() const {
	return priority;
}
void Queue::setPriority(Priority newPriority) {
	priority = newPriority;
}

Queue& Queue::operator=(const Queue& other) {
	if (this != &other) {
		this->head = other.head;
		this->tail = other.tail;
		priority = other.priority;
		weight = other.weight;
		currentWeight = other.currentWeight;
		maxOfMessages = other.maxOfMessages;
		numOfMessages = other.numOfMessages;
	}
	return *this;
}

int Queue::GetSize() {
	return pass();
}
void Queue::DecreaseWeight() {
	this->currentWeight -= 1;
}
int Queue::GetCurrentWeight() const {
	return currentWeight;
}