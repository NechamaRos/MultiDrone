#include <stdio.h>
#include <stdlib.h>
#include "cacheManagement.h"
#include <stdbool.h>

extern controlBlock_t* controlBlock;

// to create a new node of range
Node_t* CreateRangeInDataStorageNode(int location, int sizeOfBytes) {
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

// for add new range to the linkedList in the end of it
void InsertInTailOfLinkedList(Node_t** tail, int location, int sizeOfBytes) {
	if (tail == NULL || *tail == NULL)//if the lst is empty
		return;
	Node_t* newNode = CreateRangeInDataStorageNode(location, sizeOfBytes);
	(*tail)->next = newNode;
	*tail = newNode;
}

void PrintLinkedList(Node_t* head) {
	Node_t* temp = head;
	while (temp != NULL) {
		printf("Location: %d, SizeOfBytes: %d -> ", temp->data.location, temp->data.sizeOfBytes); // הדפסת הנתון של הנקודה
		temp = (Node_t*)temp->next; // מעבר לנקודה הבאה
	}
	printf("NULL\n"); // סוף הרשימה
}

// for free all the linkedList memory
void FreeLinkedList(Node_t* head) {
	Node_t* temp;
	RangeInDataStorage_t* r;
	bool b = false;
	//the first node contain id and size so we dont need to add it to the trees
	head = head->next;
	while (head != NULL) {
		temp = head;
		r = (RangeInDataStorage_t*)temp;
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, r);
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, r);
		printf(" %d \n", ((RangeInDataStorage_t*)(controlBlock->emptyPlacesByLocation->root->data))->sizeOfBytes);
		head = (Node_t*)head->next;
	}
	PrintRangeInDataStorageTree(controlBlock->emptyPlacesByLocation);
	PrintRangeInDataStorageTree(controlBlock->emptyPlacesBySize);
}


