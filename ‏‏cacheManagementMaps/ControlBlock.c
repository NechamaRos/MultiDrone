#include "cacheManagement.h"
#include <stdio.h>
#include <stdlib.h>

controlBlock_t* controlBlock;

//initial and allocate the ctrlblk
void InitControlBlock() {
	// Allocate memory for the controlBlock
	controlBlock = (controlBlock_t*)malloc(sizeof(controlBlock_t));
	if (controlBlock == NULL) 	// Handle allocation failure
		return;
	//initial queue array
	for (int i = 0; i < SIZE_OF_QUEUE_ARRAY; i++)
	{
		controlBlock->QueuesArray[i] = CreateQueue();
	}
	//initial emptyPlacesByLocation tree
	controlBlock->emptyPlacesByLocation = (AVLTree_t*)malloc(sizeof(AVLTree_t));
	controlBlock->emptyPlacesByLocation->compare = CompareRangeByLocation;
	controlBlock->emptyPlacesByLocation->root = NULL;

	//initial emptyPlacesBySize tree
	controlBlock->emptyPlacesBySize = (AVLTree_t*)malloc(sizeof(AVLTree_t));
	controlBlock->emptyPlacesBySize->compare = CompareRangeBySize;
	controlBlock->emptyPlacesBySize->root = NULL;

	//initial MapsSortedByID tree
	controlBlock->MapsSortedByID = (AVLTree_t*)malloc(sizeof(AVLTree_t));
	controlBlock->MapsSortedByID->compare = CompareMapInfo;
	controlBlock->MapsSortedByID->root = NULL;

	//initial counter of empty place to the size of cache
	controlBlock->EmptyPlaceInCache = SIZE_OF_CACHE;
}

//for delete all the data from data structures in the ctrlblk when i have to read map bigger then cache size
void ResetCache() 
{
	FreeMapInfoTree(controlBlock->MapsSortedByID->root);
	/*free(controlBlock->MapsSortedByID);
	free(controlBlock->emptyPlacesByLocation);
	free(controlBlock->emptyPlacesBySize);*/

	//delete the trees data
	controlBlock->emptyPlacesByLocation->root = NULL;
	controlBlock->emptyPlacesBySize->root = NULL;
	controlBlock->MapsSortedByID->root = NULL;

	//delete the queue array data
	for (int i = 0; i < SIZE_OF_QUEUE_ARRAY; i++)
	{
		FreeQueue(controlBlock->QueuesArray[i]);
	}

	//reallocate the queue array data
	controlBlock->EmptyPlaceInCache = SIZE_OF_CACHE;
	for (int i = 0; i < SIZE_OF_QUEUE_ARRAY; i++)
	{
		controlBlock->QueuesArray[i] = CreateQueue();
	}
}
