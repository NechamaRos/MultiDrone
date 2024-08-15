#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Queue.h"
#include "Message.h"
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Priority.h"
#include <map>

using namespace std;
#pragma region tests �� ��������� �� ������
TEST_CASE("Testing the push action into queue") {
	Message m;
	Queue q;
	q.push(m);
	CHECK(q.GetSize() == 1);
}

//���� ���� ����� �� ��� ������� ���..
TEST_CASE("Testing the pop action into queue") {
	Message m;
	Queue q;
	q.push(m);
	CHECK(q.GetSize() == 1);
	q.pop();
	CHECK(q.GetSize() == 0);
}


TEST_CASE("Testing the pass action into queue") {
	Message* m1 = new Message();
	Message* m2 = new Message();
	Queue q;
	q.push(*m1);
	q.push(*m2);
	CHECK(q.pass() == 2);
}

TEST_CASE("Testing the pick action into queue") {
	Queue q;
	Message* m1 = new Message();
	Message* m2 = new Message();
	Message* m3 = new Message();
	q.push(*m1);
	q.push(*m2);
	q.push(*m3);

	int size = 3;
	int randomIndex = std::rand() % size;
	Node* pickedNode = q.pick(randomIndex);
	MessageType pickedData = pickedNode->data.messageType;
	delete pickedNode;
	//����� �� ����� ����
	switch (randomIndex) {
	case 0:
		CHECK(pickedData == 1);
		break;
	case 1:
		CHECK(pickedData == 2);
		break;
	case 2:
		CHECK(pickedData == 3);
		break;
	}
}
TEST_CASE("Testing the mutex") {
	Queue q;
	Message m1(1);
	Message m2(2);

	std::thread t1(&Queue::push, &q, m1);
	std::thread t2(&Queue::push, &q, m2);

	t1.join();
	t2.join();

	CHECK(q.GetSize() == 2);
}

#pragma endregion


//Stub readData function for testing
Message* readData() {
	Message* message = new Message(static_cast<MessageType>(rand() % 15));
	return message;
}

// Initialize global variables
Queue* queues[3];
map<Priority, int> keysForPriorities;
mutex mtx;
vector<int> arr;
int countArr[3]{ 0, 0, 0 };

int getPriorityIndex(Priority priority) {
	switch (priority) {
	case Priority::Low: return 0;
	case Priority::MediumLow: return 1;
	case Priority::Medium: return 2;
	case Priority::MediumHigh: return 3;
	case Priority::High: return 4;
	default: return -1;
	}
}

void listenToPort(int port) {
	int randNum = 5 + rand() % 6;
	for (int i = 0; i < randNum; ++i) {
		Message* message = readData();
		Priority priority = message->GetPriority();
		int pr = getPriorityIndex(priority);
		if (pr == -1) {
			cerr << "Invalid priority: " << pr << endl;
			delete message;
			continue;
		}
		{
			lock_guard<mutex> lock(mtx);
			queues[keysForPriorities[priority]]->push(*message);
			arr.push_back(pr);
		}
		delete message;
	}
}

void init(Queue* qus, int size) {
	for (int i = 0; i < size; i++) {
		Priority p = qus[i].GetPriority();
		queues[i] = new Queue(qus[i]);
		keysForPriorities.insert(make_pair(p, i));
	}
}

TEST_CASE("Testing the init function and queue initialization") {
	Queue tempQueues[3] = { Queue(Priority::Low), Queue(Priority::Medium), Queue(Priority::High) };
	init(tempQueues, 3);

	for (int i = 0; i < 3; ++i) {
		CHECK(queues[i] != nullptr);
		CHECK(queues[i]->GetPriority() == tempQueues[i].GetPriority());
	}

	CHECK(keysForPriorities[Priority::Low] == 0);
	CHECK(keysForPriorities[Priority::Medium] == 1);
	CHECK(keysForPriorities[Priority::High] == 2);

	// Clean up allocated memory
	for (int i = 0; i < 3; ++i) {
		delete queues[i];
	}
}

TEST_CASE("Testing the listenToPort function") {
	// Initialize queues and keysForPriorities
	Queue tempQueues[3] = { Queue(Priority::Low), Queue(Priority::Medium), Queue(Priority::High) };
	init(tempQueues, 3);

	int port = 8080;
	thread listenerThread(listenToPort, port);
	listenerThread.join();

	for (int num : arr) {
		if (num >= 0 && num <= 2) {
			countArr[num]++;
		}
	}

	CHECK(queues[0]->GetSize() == countArr[0]);
	CHECK(queues[1]->GetSize() == countArr[1]);
	CHECK(queues[2]->GetSize() == countArr[2]);

	// Clean up allocated memory
	for (int i = 0; i < 3; ++i) {
		delete queues[i];
	}
}
