
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest/doctest.h"
#include <stdio.h>

extern "C" {
#include "cacheManagement.h"
	extern controlBlock_t* controlBlock;

	int id = 777;
	int size = 400;

	void printQueue(Queue_t* queue) {
		QueueNode_t* current = queue->front;
		printf("queue - \n");
		while (current != NULL) {
			PrintLinkedList((current->data).linkedList);
			printf("MapID: %d, MapSizeInBytes: %d\n", current->data.mapID,
				current->data.mapSizeInBytes);
			current = current->next;
		}
		printf("\n");
	}

	TEST_CASE("Init") {
		InitControlBlock();
	}

	TEST_CASE("ReadNormalMap")
	{
		//Fill in the data the avlTrees of the spaces in cache

		RangeInDataStorage_t r1;
		r1.location = 10;
		r1.sizeOfBytes = 495;
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, &r1);
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r1);
	
		RangeInDataStorage_t r2;
		r2.location = 0;
		r2.sizeOfBytes = 5;
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, &r2);
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r2);
		
		RangeInDataStorage_t r3;
		r3.location = 7;
		r3.sizeOfBytes = 1;
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, &r3);
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r3);

		RangeInDataStorage_t r4;
		r4.location = 5;
		r4.sizeOfBytes = 1;
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r4);
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, &r4);
		

		RangeInDataStorage_t r5;
		r5.location = 8;
		r5.sizeOfBytes = 5;
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r5);
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, &r5);

		//Fill in the data the avlTree of maps sorted by id, and the Queues Array

		Node_t* lst = CreateRangeInDataStorageNode(8, 1024);
		Node_t* tail = lst;
		InsertInTailOfLinkedList(&tail, 40, 5);
		InsertInTailOfLinkedList(&tail, 45, 5);

		MapInfo_t* mapInfoL = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mapInfoL->mapID = 8;
		mapInfoL->mapSizeInBytes = 1024;
		mapInfoL->linkedList = lst;

		RemoveMapFromCache(8);
		AddNodeToAvlTree(controlBlock->MapsSortedByID, mapInfoL);
		InsertToQueuesArray(mapInfoL);


		Node_t* l1 = CreateRangeInDataStorageNode(5, 100);
		Node_t* t1 = l1;
		InsertInTailOfLinkedList(&t1, 30, 5);
		InsertInTailOfLinkedList(&t1, 35, 5);

		MapInfo_t* mapInfoS = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mapInfoS->mapID = 5;
		mapInfoS->mapSizeInBytes = 100;
		mapInfoS->linkedList = l1;

		AddNodeToAvlTree(controlBlock->MapsSortedByID, mapInfoS);
		InsertToQueuesArray(mapInfoS);


		Node_t* l2 = CreateRangeInDataStorageNode(67, 100);
		Node_t* t2 = l2;
		InsertInTailOfLinkedList(&t2, 20, 5);
		InsertInTailOfLinkedList(&t2, 25, 5);

		MapInfo_t* mapInfoM = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mapInfoM->mapID = 67;
		mapInfoM->mapSizeInBytes = 100;
		mapInfoM->linkedList = l2;

		AddNodeToAvlTree(controlBlock->MapsSortedByID, mapInfoM);
		InsertToQueuesArray(mapInfoM);

		controlBlock->EmptyPlaceInCache = 10;
		
		ReadNormalMap(22,500);
		PrintMapInfoTree(controlBlock->MapsSortedByID);
		PrintRangeInDataStorageTree(controlBlock->emptyPlacesByLocation);
		PrintRangeInDataStorageTree(controlBlock->emptyPlacesBySize);

	}


	TEST_CASE("TestLinkedList") 
	{

		CHECK(CreateRangeInDataStorageNode(0, 5)->data.location == 0);
		CHECK(CreateRangeInDataStorageNode(0, 5)->data.sizeOfBytes == 5);
		Node_t* lst = CreateRangeInDataStorageNode(id, size);
		Node_t* tail = lst;
		InsertInTailOfLinkedList(&tail, 0, 5);
		printf("%d\n", tail->data.location);
		InsertInTailOfLinkedList(&tail, 9, 10);
		printf("%d\n", tail->data.location);
		PrintLinkedList(lst);
	}


	TEST_CASE("TestRangeInDataStorageByLocation") 
	{
		ResetCache();

		printf("TestRangeInDataStorageByLocation\n");
		RangeInDataStorage_t r1;
		r1.location = 10;
		r1.sizeOfBytes = 3;
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, &r1);
		CHECK(Abs(Height(controlBlock->emptyPlacesByLocation->root->left) - Height(controlBlock->emptyPlacesByLocation->root->right)) <= 1);

		RangeInDataStorage_t r2;
		r2.location = 0;
		r2.sizeOfBytes = 5;
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, &r2);
		CHECK(Abs(Height(controlBlock->emptyPlacesByLocation->root->left) - Height(controlBlock->emptyPlacesByLocation->root->right)) <= 1);

		RangeInDataStorage_t r3;
		r3.location = 7;
		r3.sizeOfBytes = 1;
		AddNodeToAvlTree(controlBlock->emptyPlacesByLocation, &r3);
		CHECK(Abs(Height(controlBlock->emptyPlacesByLocation->root->left) - Height(controlBlock->emptyPlacesByLocation->root->right)) <= 1);

#pragma region check that each node in the right location in tree
		CHECK(controlBlock->emptyPlacesByLocation->root->data == &r3);
		CHECK(controlBlock->emptyPlacesByLocation->root->left->data == &r2);
		CHECK(controlBlock->emptyPlacesByLocation->root->right->data == &r1);
#pragma endregion
		CHECK(FindRangeByLocation(controlBlock->emptyPlacesByLocation->root, 0)->data == &r2);
		CHECK_FALSE(FindRangeByLocation(controlBlock->emptyPlacesByLocation->root, 0)->data == &r3);

		//after I Delete the root, the new root has to be r1

		DeleteNodeFromEmptyPlacesByLocation(7);
		CHECK(FindRangeByLocation(controlBlock->emptyPlacesByLocation->root, 7) == NULL);
		CHECK(controlBlock->emptyPlacesByLocation->root->data == &r1);
		CHECK(controlBlock->emptyPlacesByLocation->root->left->data == &r2);
		CHECK(controlBlock->emptyPlacesByLocation->root->right == NULL);

		UpdateNodeInRangeByLocation((AVLNode_t*)&r2, 3);
		CHECK(((RangeInDataStorage_t*)(controlBlock->emptyPlacesByLocation->root->left->data))->sizeOfBytes == 3);

		PrintRangeInDataStorageTree(controlBlock->emptyPlacesByLocation);
	}


	TEST_CASE("TestRangeInDataStorageBySize") 
	{

		printf("TestRangeInDataStorageBySize");
		RangeInDataStorage_t r1;
		r1.location = 10;
		r1.sizeOfBytes = 3;
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r1);
		CHECK(Abs(Height(controlBlock->emptyPlacesBySize->root->left) - Height(controlBlock->emptyPlacesBySize->root->right)) <= 1);
		
		RangeInDataStorage_t r2;
		r2.location = 0;
		r2.sizeOfBytes = 5;
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r2);
		CHECK(Abs(Height(controlBlock->emptyPlacesBySize->root->left) - Height(controlBlock->emptyPlacesBySize->root->right)) <= 1);

		RangeInDataStorage_t r3;
		r3.location = 7;
		r3.sizeOfBytes = 1;
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r3);
		CHECK(Abs(Height(controlBlock->emptyPlacesBySize->root->left) - Height(controlBlock->emptyPlacesBySize->root->right)) <= 1);

		RangeInDataStorage_t r4;
		r4.location = 5;
		r4.sizeOfBytes = 1;
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r4);
		CHECK(Abs(Height(controlBlock->emptyPlacesBySize->root->left) - Height(controlBlock->emptyPlacesBySize->root->right)) <= 1);

		RangeInDataStorage_t r5;
		r5.location = 8;
		r5.sizeOfBytes = 5;
		AddNodeToAvlTree(controlBlock->emptyPlacesBySize, &r5);
		CHECK(Abs(Height(controlBlock->emptyPlacesBySize->root->left) - Height(controlBlock->emptyPlacesBySize->root->right)) <= 1);

#pragma region check that eact node in the right location in tree
		CHECK(controlBlock->emptyPlacesBySize->root->data == &r1);
		CHECK(controlBlock->emptyPlacesBySize->root->left->data == &r3);
		CHECK(controlBlock->emptyPlacesBySize->root->right->data == &r2);

		PrintRangeInDataStorageTree(controlBlock->emptyPlacesBySize);

		UpdateNodeInRangeBySize(&r2, 0);
		CHECK(r2.sizeOfBytes == 0);


#pragma endregion
		
		DeleteNodeFromEmptyPlacesBySize(&r1);

		PrintRangeInDataStorageTree(controlBlock->emptyPlacesBySize);

		//לעשות את המיזוג של הלוקיישנים עי שימוש בעיקרון של סריקה תוכית
	}



	TEST_CASE("TestMapInfoTree")
	{
		ResetCache();

		// יצירת עץ AVL עבור MapInfo
		//linkedList for the mapInfo
		Node_t* lst = CreateRangeInDataStorageNode(id, size);
		Node_t* tail = lst;
		InsertInTailOfLinkedList(&tail, 0, 5);
		InsertInTailOfLinkedList(&tail, 9, 10);


		MapInfo_t* mapInfoL = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mapInfoL->mapID = 8;
		mapInfoL->mapSizeInBytes = 1024;
		mapInfoL->linkedList = lst;

		MapInfo_t* mapInfoS = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mapInfoS->mapID = 5;
		mapInfoS->mapSizeInBytes = 100;
		mapInfoS->linkedList = lst;

		MapInfo_t* mapInfoM = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mapInfoM->mapID = 7;
		mapInfoM->mapSizeInBytes = 200;
		mapInfoM->linkedList = lst;

		AddNodeToAvlTree(controlBlock->MapsSortedByID, mapInfoL);
		CHECK(Abs(Height(controlBlock->MapsSortedByID->root->left) - Height(controlBlock->MapsSortedByID->root->right)) <= 1);
		AddNodeToAvlTree(controlBlock->MapsSortedByID, mapInfoS);
		CHECK(Abs(Height(controlBlock->MapsSortedByID->root->left) - Height(controlBlock->MapsSortedByID->root->right)) <= 1);
		AddNodeToAvlTree(controlBlock->MapsSortedByID, mapInfoM);
		CHECK(Abs(Height(controlBlock->MapsSortedByID->root->left) - Height(controlBlock->MapsSortedByID->root->right)) <= 1);

		//add each node synchronized to MapsSortedByID and to
#pragma region check that eact node in the righr location in tree
		CHECK(controlBlock->MapsSortedByID->root->data == mapInfoM);
		CHECK(controlBlock->MapsSortedByID->root->left->data == mapInfoS);
		CHECK(controlBlock->MapsSortedByID->root->right->data == mapInfoL);
#pragma endregion
		CHECK(FindMapInfoByID(controlBlock->MapsSortedByID->root, 8)->data == mapInfoL);


		//CHECK(FindMapInfoByID(controlBlock->MapsSortedByID->root, 8)->data == NULL);

		//printMapInfoTree(controlBlock->MapsSortedByID);

		//after i Delete the root, the new root has to be mapInfoL
		//CHECK(RecursiveDeleteMapFromMapsSortedByID(controlBlock->MapsSortedByID->root, 7)->data == mapInfoL);
	/*	CHECK(controlBlock->MapsSortedByID->root->data == mapInfoL);
		CHECK(FindMapInfoByID(controlBlock->MapsSortedByID->root, 7) == NULL);
		CHECK(controlBlock->MapsSortedByID->root->left->data == mapInfoS);
		CHECK(FindRangeByLocation(controlBlock->emptyPlacesByLocation->root, 9) != NULL);*/
	}



	TEST_CASE("queueTest") {
		ResetCache();
		InitControlBlock();

		Node_t* lst = CreateRangeInDataStorageNode(id, size);
		Node_t* tail = lst;
		InsertInTailOfLinkedList(&tail, 7, 10);
		InsertInTailOfLinkedList(&tail, 2, 4);

		MapInfo_t* mi = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mi->mapID = 7;
		mi->mapSizeInBytes = 256;
		mi->linkedList = lst;

		MapInfo_t* mi1 = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mi1->mapID = 5;
		mi1->mapSizeInBytes = 55;
		mi1->linkedList = lst;

		MapInfo_t* mi2 = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mi2->mapID = 1;
		mi2->mapSizeInBytes = 54;
		mi2->linkedList = lst;


		Queue_t* q = CreateQueue();
		Enqueue(q, *mi);
		Enqueue(q, *mi1);

		InsertToQueuesArray(mi);
		InsertToQueuesArray(mi2);
		InsertToQueuesArray(mi1);


		//insertToQueueArray(mi);
		for (int i = 0; i < SIZE_OF_QUEUE_ARRAY; i++)
		{
			printf("%d- \n", i);
			printQueue(controlBlock->QueuesArray[i]);
		}
	}
}
TEST_CASE("readMapBiggerThanCacheTest") 
{
	ReadMapBiggerThanCache(24, 1000);
}