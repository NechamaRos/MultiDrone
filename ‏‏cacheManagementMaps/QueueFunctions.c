#include "cacheManagement.h"
#include <stdio.h>
#include <stdlib.h>

extern controlBlock_t* controlBlock;

//Function to create a new queue
Queue_t* CreateQueue() {
	Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
	queue->front = NULL;
	queue->rear = NULL;
	return queue;
}

//Add to queue function
void Enqueue(Queue_t* queue, MapInfo_t mapInfo) {
	QueueNode_t* newNode = (QueueNode_t*)malloc(sizeof(QueueNode_t));
	newNode->data = mapInfo;
	newNode->next = NULL;

	if (queue->rear == NULL) {
		//If the queue is empty
		queue->front = newNode;
		queue->rear = newNode;
	}
	else {
		// Add an item at the end of the queue
		queue->rear->next = newNode;
		queue->rear = newNode;
	}
}

// Dequeue function
MapInfo_t* Dequeue(Queue_t* queue) {
	if (queue->front == NULL) {
		fprintf(stderr, "Queue is empty\n");
		exit(EXIT_FAILURE);
	}

	QueueNode_t* temp = queue->front;
	MapInfo_t data = temp->data;

	queue->front = queue->front->next;
	if (queue->front == NULL) {
		queue->rear = NULL;
	}

	free(temp);
	return &data;
}

// Function to check if the queue is empty
int IsQueueEmpty(Queue_t* queue) {
	return (queue->front == NULL);
}

//A function to release all queue memory
void FreeQueue(Queue_t* queue) {
	if (queue)
	{
		while (!IsQueueEmpty(queue)) {
			Dequeue(queue);
		}
		free(queue);
	}
}

//A function that calculates according to the size of the map at which index of the array of queues 
//it should be inserted
int CalculateIndexInQueuesArray(int size) {
	int index = size / SIZE_OF_RANGE_IN_QUEUE_ARRAY;
	if (index >= SIZE_OF_QUEUE_ARRAY)
		index = SIZE_OF_QUEUE_ARRAY - 1;
	printf("index in array - %d \n", index);
	return index;
}

//An insert function to the array of queues
void InsertToQueuesArray(MapInfo_t* mapInfo) {
	//insert the map to the match queue by the calculation
	Enqueue(controlBlock->QueuesArray[CalculateIndexInQueuesArray(mapInfo->mapSizeInBytes)], *mapInfo);
}

//A function to output the largest map entered recently
MapInfo_t* RemoveMaxMapFromQueuesArray() 
{
	
	int indexInQueueArray = SIZE_OF_QUEUE_ARRAY - 1;
	MapInfo_t* temp = (MapInfo_t*)malloc(sizeof(MapInfo_t));

	//As long as I'm within range of deviate from the array of queues
	while (indexInQueueArray >= 0)
	{
		//If the head of the queue is empty, go to the queue indexed below
		if (controlBlock->QueuesArray[indexInQueueArray]->front == NULL) {
			indexInQueueArray--;
		}
		else
		{
			//Give me back the head of the queue
			temp = Dequeue(controlBlock->QueuesArray[indexInQueueArray]);
			//If the member's linked list has not already been freed by the sorted tree, return the member
			if (temp->linkedList->data.location == temp->mapID)//location in first linkedList is mpaID
			{
				return temp;
			}
		}
	}
	return NULL;
}
