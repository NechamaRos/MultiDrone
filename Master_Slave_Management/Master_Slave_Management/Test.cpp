#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Message.h"
#include "Queue.h"
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
//using namespace std;


TEST_CASE("Testing the push action into queue") {
	Message m;
	Queue q;
	q.push(m);
	CHECK(q.GetSize() == 1);
}

//אולי אפשר לבדוק את שתי הפעולות יחד..
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
	//לבדוק אם הנכון הוצא
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

using namespace std;

Message readData() {
	Message message;
	message.setPriority(1);
	return message;
}

// Initialize queues
Queue* queues[3] = { new Queue(0), new Queue(1), new Queue(2) };

mutex mtx;
vector<int> arr;  // Use vector for dynamic sizing
int countArr[3]{ 0,0,0 };

// Initialize random seed once
void initializeRandom() {
	static bool seeded = false;
	if (!seeded) {
		srand(time(0));  // Seed the random number generator
		seeded = true;
	}
}

int getRandomNumber() {
	initializeRandom();
	return rand() % 3 + 1;  // Random number between 1 and 3
}

void listenToPort(int port) {
	for (int i = 0; i < 10; ++i) {
		Message message = readData();
		int priority = getRandomNumber();
		if (priority < 1 || priority > 3) {
			cerr << "Invalid priority: " << priority << endl;
			continue;
		}
		else {
			lock_guard<mutex> lock(mtx);
			queues[priority - 1]->push(message);  // Corrected priority index
			arr.push_back(priority);  // Use vector for dynamic size
		}
	}
}

TEST_CASE("Testing the listenToPort function") {
	int port = 8080;
	thread listenerThread(listenToPort, port);
	listenerThread.join();

	// Count occurrences of each number in arr
	for (int num : arr) {
		if (num >= 1 && num <= 3) {
			countArr[num - 1]++;
		}
	}

	// Check if the queue sizes match the countArr values
	CHECK(queues[0]->GetSize() == countArr[0]);
	CHECK(queues[1]->GetSize() == countArr[1]);
	CHECK(queues[2]->GetSize() == countArr[2]);
}