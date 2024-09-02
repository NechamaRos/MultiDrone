#include <stdio.h>
#include <stdlib.h>
#include "cacheManagement.h"

#pragma region Node & LnkdLst function
// to create a new node of mapInfo
Node_t* createNode(int location, int sizeOfBytes) {
	Node_t* newNode = (Node_t*)malloc(sizeof(Node_t)); // allocate memory for the node
	if (newNode == NULL) {
		printf("Error allocating memory\n");
		exit(1);
	}
	newNode->data.location = location;
	newNode->data.sizeOfBytes = sizeOfBytes;
	newNode->next = NULL;
	return newNode;
}

// for add new mapInfo to the linkedList in the end of it
void insertInTail(Node_t** tail, int location, int sizeOfBytes) {
	if (tail == NULL || *tail == NULL)//if the lst is empty
		return;
	Node_t* newNode = createNode(location, sizeOfBytes);
	(*tail)->next = newNode;
	*tail = newNode;
}

void printLinkedList(Node_t* head) {
	Node_t* temp = head;
	while (temp != NULL) {
		printf("Location: %d, SizeOfBytes: %d -> ", temp->data.location, temp->data.sizeOfBytes); // הדפסת הנתון של הנקודה
		temp = (Node_t*)temp->next; // מעבר לנקודה הבאה
	}
	printf("NULL\n"); // סוף הרשימה
}

// for free all the linkedList memory
void freeLinkedList(Node_t* head) {
	Node_t* temp;
	while (head != NULL) {
		temp = head;
		deleteNodeFromEmptyPlacesByLocation(temp->data.location);
		deleteNodeFromEmptyPlacesBySize(&temp->data);
		head = (Node_t*)head->next;
		free(temp);
	}
}
#pragma endregion

