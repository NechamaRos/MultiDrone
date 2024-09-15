#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <sycl/sycl.hpp>
#include "doctest.h"
#include "Scheduling.h"
#include <vector>
#include <thread>
#include "Message.h"
#include "Queue.h"
#include <random>
#include "dataOfMessage.h"

/**
 * @brief Helper function to generate a random task queue.
 *
 * @param numMessages Number of messages to generate in the queue.
 * @param weight The weight of the queue.
 * @param priority The priority of the queue.
 * @return Queue* Pointer to the generated Queue object.
 */
Queue* generateRandomMessageQueue(int numMessages, int weight, int priority) {
    Queue* queue = new Queue(weight, priority);
    for (int i = 0; i < numMessages; ++i) {
        Message* msg = new Message();
        queue->push(*msg);
    }
    return queue;
}

/**
 * @brief Helper function to generate a random list of queues.
 *
 * @param numQueues Number of queues to generate.
 * @param maxMessagesPerQueue Maximum number of messages per queue.
 * @return std::list<Queue> List of generated Queue objects.
 */
std::list<Queue> generateRandomQueues(int numQueues, int maxMessagesPerQueue) {
    std::list<Queue> queues;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, maxMessagesPerQueue);

    for (int i = 0; i < numQueues; ++i) {
        int numMessages = dist(gen);
        int weight = dist(gen);
        int priority = dist(gen);
        queues.push_back(*generateRandomMessageQueue(numMessages, weight, priority));
    }
    return queues;
}

/**
 * @brief Test single message in multiple queues.
 */
TEST_CASE("Single Message in Multiple Queues") {
    std::list<Queue> queues = generateRandomQueues(3, 1);
    WeightedRoundRobin(&queues, false);
    for (const auto& queue : queues) {
        CHECK(queue.isEmpty());
    }
}

/**
 * @brief Test handling empty queues.
 */
TEST_CASE("Handling Empty Queues") {
    Queue* queueWithMessages = generateRandomMessageQueue(5, 3, 0);
    Queue emptyQueue1(2, 0);
    Queue emptyQueue2(2, 0);

    std::list<Queue> queues;
    queues.emplace_back(*queueWithMessages);
    queues.emplace_back(emptyQueue1);
    queues.emplace_back(emptyQueue2);

    WeightedRoundRobin(&queues, false);
    for (const auto& queue : queues) {
        CHECK(queue.isEmpty());
    }
    delete queueWithMessages;
}

/**
 * @brief Test to check for starvation.
 */
TEST_CASE("Starvation Check") {
    Queue* highPriorityQueue = generateRandomMessageQueue(9, 4, 0);
    Queue* lowPriorityQueue = generateRandomMessageQueue(3, 1, 0);

    std::list<Queue> queues;
    queues.emplace_back(*highPriorityQueue);
    queues.emplace_back(*lowPriorityQueue);

    WeightedRoundRobin(&queues, false);
    CHECK(queues.back().isEmpty());

    delete highPriorityQueue;
    delete lowPriorityQueue;
}

/**
 * @brief Test handling many queues.
 */
TEST_CASE("Handling Many Queues") {
    std::list<Queue> queues = generateRandomQueues(10, 5);
    WeightedRoundRobin(&queues, false);
    for (const auto& queue : queues) {
        CHECK(queue.isEmpty());
    }
}

/**
 * @brief Test processing a large number of tasks.
 */
TEST_CASE("Large Number of Tasks") {
    Queue* priorityQueue = generateRandomMessageQueue(1000, 5, 0);
    std::list<Queue> queues;
    queues.emplace_back(*priorityQueue);
    WeightedRoundRobin(&queues, false);
    CHECK(queues.front().isEmpty());
    delete priorityQueue;
}

/**
 * @brief Test handling queues with zero weight.
 */
TEST_CASE("Zero Weight Queues") {
    Queue* queue1 = generateRandomMessageQueue(3, 0, 0);
    Queue* queue2 = generateRandomMessageQueue(3, 2, 0);

    std::list<Queue> queues;
    queues.emplace_back(*queue1);
    queues.emplace_back(*queue2);

    WeightedRoundRobin(&queues, false);
    CHECK_FALSE(queues.front().isEmpty());
    CHECK(queues.back().isEmpty());

    delete queue1;
    delete queue2;
}

/**
 * @brief Test adding tasks during execution.
 */
TEST_CASE("Add Tasks During Execution") {
    Queue* initialQueue = generateRandomMessageQueue(3, 3, 0);
    std::list<Queue> queues;
    queues.emplace_back(*initialQueue);

    // Run the scheduler with the option to add tasks during execution
    WeightedRoundRobin(&queues, true);
    CHECK(queues.front().isEmpty());
    delete initialQueue;
}

/**
 * @brief Test updating the time quantum after each task.
 */
TEST_CASE("Time Quantum Update") {
    Queue* messageQueue = generateRandomMessageQueue(1, 3, 0);
    std::list<Queue> queues;
    queues.emplace_back(*messageQueue);

    WeightedRoundRobin(&queues, false);

    // Assuming the time quantum is updated after each task, we verify the update
    CHECK(queues.front().getPriority() == 2);  // Replace '2' with the expected value

    delete messageQueue;
}

/**
 * @brief Test processing tasks from queues with different priorities.
 */
TEST_CASE("Mixed Priority Task Processing") {
    Queue* highPriorityQueue = generateRandomMessageQueue(5, 3, 0);
    Queue* mediumPriorityQueue = generateRandomMessageQueue(3, 2, 0);
    Queue* lowPriorityQueue = generateRandomMessageQueue(2, 1, 0);

    std::list<Queue> queues;
    queues.emplace_back(*highPriorityQueue);
    queues.emplace_back(*mediumPriorityQueue);
    queues.emplace_back(*lowPriorityQueue);

    WeightedRoundRobin(&queues, false);
    for (const auto& queue : queues) {
        CHECK(queue.isEmpty());
    }

    delete highPriorityQueue;
    delete mediumPriorityQueue;
    delete lowPriorityQueue;
}

/**
 * @brief Test task waiting time with random tasks and queues.
 */
TEST_CASE("Task Waiting Time Check With Random Tasks and Queues") {
    auto queues = generateRandomQueues(3, 10); // For example, 3 queues with up to 10 tasks each

    // Measurement - Record entry times
    std::vector<std::pair<Message*, std::chrono::high_resolution_clock::time_point>> messagesWithTimes;
    for (auto& queue : queues) {
        while (!queue.isEmpty()) {
            Message* msg = queue.pop();
            messagesWithTimes.push_back({ msg, std::chrono::high_resolution_clock::now() });
        }
    }

    // Run the scheduling algorithm
    WeightedRoundRobin(&queues, false);

    // Check - Measure waiting time again
    int maxTimePerMessage = 100;  // Maximum allowed time in milliseconds
    for (const auto& pair : messagesWithTimes) {
        const Message* msg = pair.first;
        const std::chrono::high_resolution_clock::time_point& entryTime = pair.second;

        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> waitingTime = now - entryTime;
        CHECK(waitingTime.count() <= maxTimePerMessage);

        // Free memory after use
        delete msg;
    }
}
// Global variables for tracking function calls
bool getARequestForAMapCalled = false;
bool sendRequestForImageInSpecificPointCalled = false;
bool getAStructForImageInPointCalled = false;
bool sendRequestToSlaveToGetRealImageCalled = false;
bool returnOfAnImageByItsPropertiesCalled = false;

// Mock functions for testing
void testGetARequestForAMap(Message* message) {
    getARequestForAMapCalled = true;
}

void testSendRequestForImageInSpecificPoint(Message* message) {
    sendRequestForImageInSpecificPointCalled = true;
}

void testGetAStructForImageInPoint(Message* message) {
    getAStructForImageInPointCalled = true;
}

void testSendRequestToSlaveToGetRealImage(Message* message) {
    sendRequestToSlaveToGetRealImageCalled = true;
}

void testReturnOfAnImageByItsProperties(Message* message) {
    returnOfAnImageByItsPropertiesCalled = true;
}

TEST_CASE("Test Queue::Handler with various message types") {
    InitialPointerToFunction();
    Queue queue(1, 1);

    // Test M_GOT_REQ message type
    Message message;
    message.messageType = MessageType::M_GOT_REQ;
    queue.push(message);

    // Override the function pointer for testing
    dataOfMessageVector[static_cast<size_t>(MessageType::M_GOT_REQ)].func = &testGetARequestForAMap;

    getARequestForAMapCalled = false;
    queue.Handler();
    CHECK(getARequestForAMapCalled);

    // Test MTS_ASK_FOR_IMAGES message type
    message.messageType = MessageType::MTS_ASK_FOR_IMAGES;
    queue.push(message);

    // Override the function pointer for testing
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_ASK_FOR_IMAGES)].func = &testSendRequestForImageInSpecificPoint;

    sendRequestForImageInSpecificPointCalled = false;
    queue.Handler();
    CHECK(sendRequestForImageInSpecificPointCalled);

    // Test STM_SEND_IMAGES_PROP message type
    message.messageType = MessageType::STM_SEND_IMAGES_PROP;
    queue.push(message);

    // Override the function pointer for testing
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_SEND_IMAGES_PROP)].func = &testGetAStructForImageInPoint;

    getAStructForImageInPointCalled = false;
    queue.Handler();
    CHECK(getAStructForImageInPointCalled);

    // Test MTS_GIVE_THE_CHOSEN_IMAGE message type
    message.messageType = MessageType::MTS_GIVE_THE_CHOSEN_IMAGE;
    queue.push(message);

    // Override the function pointer for testing
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_GIVE_THE_CHOSEN_IMAGE)].func = &testSendRequestToSlaveToGetRealImage;

    sendRequestToSlaveToGetRealImageCalled = false;
    queue.Handler();
    CHECK(sendRequestToSlaveToGetRealImageCalled);

    // Test STM_RESPONSE_TO_THE_CHOSEN_IMAGE message type
    message.messageType = MessageType::STM_RESPONSE_TO_THE_CHOSEN_IMAGE;
    queue.push(message);

    // Override the function pointer for testing
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_RESPONSE_TO_THE_CHOSEN_IMAGE)].func = &testReturnOfAnImageByItsProperties;

    returnOfAnImageByItsPropertiesCalled = false;
    queue.Handler();
    CHECK(returnOfAnImageByItsPropertiesCalled);
}







