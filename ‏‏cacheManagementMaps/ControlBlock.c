#include "cacheManagement.h"
#include <stdio.h>
#include <stdlib.h>

controlBlock_t* controlBlock;

void initControlBlock() {
	// Allocate memory for the controlBlock
	controlBlock = (controlBlock_t*)malloc(sizeof(controlBlock_t));
	if (controlBlock == NULL) {
		// Handle allocation failure
		return;
	}
	for (int i = 0; i < SIZE_OF_QUEUE_ARRAY; i++)
	{
		controlBlock->queueArray[i] = createQueue();
	}
	controlBlock->emptyPlacesByLocation = (AVLTree_t*)malloc(sizeof(AVLTree_t));
	controlBlock->emptyPlacesByLocation->compare = compareRangeByLocation;
	controlBlock->emptyPlacesByLocation->root = NULL;

	controlBlock->emptyPlacesBySize = (AVLTree_t*)malloc(sizeof(AVLTree_t));
	controlBlock->emptyPlacesBySize->compare = compareRangeBySize;
	controlBlock->emptyPlacesBySize->root = NULL;
}
