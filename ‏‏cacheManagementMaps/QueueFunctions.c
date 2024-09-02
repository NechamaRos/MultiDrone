#include "cacheManagement.h"
#include <stdio.h>
#include <stdlib.h>

extern controlBlock_t* controlBlock;

// פונקציה ליצירת תור חדש
Queue_t* createQueue() {
	Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
	queue->front = NULL;
	queue->rear = NULL;
	return queue;
}

// פונקציה להוספת פריט לתור (enqueue)
void enqueue(Queue_t* queue, MapInfo_t mapInfo) {
	QueueNode_t* newNode = (QueueNode_t*)malloc(sizeof(QueueNode_t));
	newNode->data = mapInfo;
	newNode->next = NULL;

	if (queue->rear == NULL) {
		// אם התור ריק
		queue->front = newNode;
		queue->rear = newNode;
	}
	else {
		// הוספת פריט בסוף התור
		queue->rear->next = newNode;
		queue->rear = newNode;
	}
}

// פונקציה להסרת פריט מהתור (dequeue)
MapInfo_t* dequeue(Queue_t* queue) {
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

// פונקציה לבדוק אם התור ריק
int isQueueEmpty(Queue_t* queue) {
	return (queue->front == NULL);
}

// פונקציה לשחרור זיכרון התור
void freeQueue(Queue_t* queue) {
	while (!isQueueEmpty(queue)) {
		dequeue(queue);
	}
	free(queue);
}

int calculateIndexInQueueArray(int size) {
	int index = size / SIZE_OF_RANGE_IN_QUEUE_ARRAY;
	if (index > SIZE_OF_QUEUE_ARRAY)
		index = SIZE_OF_QUEUE_ARRAY - 1;
	printf("index in array - %d \n", index);
	return index;
}

void insertToQueueArray(MapInfo_t* mapInfo) {
	enqueue(controlBlock->queueArray[calculateIndexInQueueArray(mapInfo->mapSizeInBytes)], *mapInfo);
}

MapInfo_t* removeMaxMapFromQueueArray() {

	int indexInQueueArray = SIZE_OF_RANGE_IN_QUEUE_ARRAY - 1;
	MapInfo_t* temp = (MapInfo_t*)malloc(sizeof(MapInfo_t));

	while (indexInQueueArray >= 0)
	{
		if (controlBlock->queueArray[indexInQueueArray]->front == NULL) {
			indexInQueueArray--;
		}
		else
		{
			temp = dequeue(controlBlock->queueArray[indexInQueueArray]);
			if (temp->linkedList->data.location == temp->mapID)//location in first linkedList is mpaID
			{
				return temp;
			}
		}
	}
	return NULL;
}
