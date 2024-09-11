#include <stdio.h>
#include <stdlib.h>
#include "cacheManagement.h"

extern controlBlock_t* controlBlock;

//Function of removing map from cache by id
void RemoveMapFromCache(int id)
{
	AVLNode_t* map;
	//The map I search
	map = FindMapInfoByID(controlBlock->MapsSortedByID->root, id);

	//If the map is found
	if (map != NULL)
	{
		//delete the map from MapsSortedByID, and free its linkedList
		DeleteNodeFromMapsSortedByID(((MapInfo_t*)map->data)->mapID);
		FreeLinkedList(((MapInfo_t*)map->data)->linkedList);

		//increase the counter of empty places in cache
		controlBlock->EmptyPlaceInCache += ((MapInfo_t*)map->data)->mapSizeInBytes;
	}
}

//Function of reading a part from map
ValuesOfReadingMap_t ReadPieceOfMap(Node_t* linkedListOfLocations, int size, int offset, int id)
{
	Node_t* linkedListTail = linkedListOfLocations;

	//item of reading map state
	ValuesOfReadingMap_t mapReadingInfo;

	AVLNode_t* emptyPlaceInLocations;

	//pop the max empty place
	AVLNode_t* maxPlace = FindMax(controlBlock->emptyPlacesBySize->root);

	RangeInDataStorage_t* placeToInsert = (RangeInDataStorage_t*)(maxPlace->data);

	//If the size of the map is smaller than the empty space to insert
	if (placeToInsert->sizeOfBytes > size)
	{

		//update the max-size node in emptyPlacesBySize with the new size
		UpdateNodeInRangeBySize(maxPlace->data, placeToInsert->sizeOfBytes - size);

		//update the max-size node in emptyPlacesByLocation with the new size
		
		//I change the pointer, and the location changed by itself

		//add node of location and size to the map locations linkedList
		InsertInTailOfLinkedList(&linkedListTail, placeToInsert->location, size);

		//fetch of insert - id, offset of map, location in cache, and size of bytes  
		FetchFromDisk(id, offset, placeToInsert->location, size);

		//set values of state of reading map

		//linkedList = linked list with new location
		mapReadingInfo.linkedList = linkedListOfLocations;

		//offset = offset + the bytes read
		mapReadingInfo.offset = offset + size;

		mapReadingInfo.BitsLeftToRead = 0;

		return mapReadingInfo;
	}
	else if (size >= placeToInsert->sizeOfBytes)
	{
		//remove the empty place from the empty places sorted by size
		DeleteNodeFromEmptyPlacesBySize(placeToInsert);

		//remove the empty place from the empty places sorted by location
		DeleteNodeFromEmptyPlacesByLocation(placeToInsert->location);

		//add node of location and size to the map locations linkedList
		InsertInTailOfLinkedList(&linkedListTail, placeToInsert->location, placeToInsert->sizeOfBytes);

		//fetch of insert - id, offset of map, location in cache, and size of bytes  
		FetchFromDisk(id, offset, placeToInsert->location, placeToInsert->sizeOfBytes);

		//set values of state of reading map

		//linkedList = linked list with new location
		mapReadingInfo.linkedList = linkedListOfLocations;

		//offset = offset + the bytes read
		mapReadingInfo.offset = offset + placeToInsert->sizeOfBytes;

		mapReadingInfo.BitsLeftToRead = size - placeToInsert->sizeOfBytes;

		return mapReadingInfo;
	}
}

//Reading a map smaller than cache
void ReadNormalMap(int id, int size)
{
	MapInfo_t* newMap = (MapInfo_t*)malloc(sizeof(MapInfo_t));
	Node_t* linkedListOfLocations = CreateRangeInDataStorageNode(id, size);
	Node_t* linkedListOfLocationsTail = linkedListOfLocations;
	int offset = 0;
	int BitsLeftToRead = size;
	ValuesOfReadingMap_t readingInfo;
	//Make sure there is enough space in the cache
	MakesRoomForCurrentIncome(size);

	//As long as I haven't finished loading the image
	while (size > offset)
	{
		//read new piece from map
		readingInfo = ReadPieceOfMap(linkedListOfLocationsTail, BitsLeftToRead, offset, id);
		//Move the linked list pointer that was filled on the last call
		linkedListOfLocationsTail = linkedListOfLocationsTail->next;
		offset = readingInfo.offset;
		BitsLeftToRead = readingInfo.BitsLeftToRead;
	}

	//set object to contain my map
	newMap->linkedList = linkedListOfLocations;
	newMap->mapID = id;
	newMap->mapSizeInBytes = size;

	//insert the map to my structures:

	//insert the map to MapsSortedByID
	AddNodeToAvlTree(controlBlock->MapsSortedByID, newMap);

	//insert the map to QueueArray
	InsertToQueuesArray(newMap);
}

//Reading a map bigger than cache
void ReadMapBiggerThanCache(int id, int mapSize) {
	int offset = 0, unReadBitsCount;

	//Delete all the data from the data structures
	ResetCache();
	//As long as the number of bits I have left to read is greater than the cache
	while (offset < mapSize) {
		unReadBitsCount = mapSize - offset;
		if (unReadBitsCount >= SIZE_OF_CACHE) {
			//Reading the map and inserting it into the entire size of the cache
			FetchFromDisk(id, offset, controlBlock->cache[0], SIZE_OF_CACHE);
			offset += SIZE_OF_CACHE;
		}

		else 
		{
			FetchFromDisk(id, offset, controlBlock->cache[0], mapSize - offset);
			offset += mapSize - offset;
		}

		//wait for user to read the data
		printf("sleep for 1 second\n");
		Sleep(1000);
	}
	//Since the data is not mapped, I don't need it and I delete it
	ResetCache();
}

//dummy function of request to read from disk
void FetchFromDisk(int id, int offset, int location, int size)
{
	return;
}

//Override function to clear space
void OverrideInternal(int size)
{
	MapInfo_t* temp = (MapInfo_t*)malloc(sizeof(MapInfo_t));
	int counter = 0;
	//As long as the size of the released maps is smaller than the size that is missing for the insertion of the requested map
	while (size > counter)
	{
		//Insert into temp the data of the largest map inserted recently
		temp = RemoveMaxMapFromQueuesArray();

		if (temp != NULL)
		{
			counter += temp->mapSizeInBytes;
			//increase the empty place in cache
			controlBlock->EmptyPlaceInCache += temp->mapSizeInBytes;
			//Adding the places that the map took to the empty places
			FreeLinkedList(temp->linkedList);
		}
	}
}

//Wrap function that makes sure there is enough space
void MakesRoomForCurrentIncome(int size) {
	if (size >= controlBlock->EmptyPlaceInCache)
	{
		OverrideInternal(size - controlBlock->EmptyPlaceInCache);
	}
}

//A function that accepts a request for a map according to 2 points longitude and latitude
void LoadMapsByBR_TL(Point_t br, Point_t tl)
{
	AVLNode_t* map;
	map_t array[SIZE_OF_QUEUE_ARRAY];
	int index = 0;
	//Sending a request to the disk to receive the ID and size of the maps corresponding to the request according to the 2 points
	
	//index = disk_mng_getMapsIdsInRange(br, tl, array, SIZE_OF_QUEUE_ARRAY, index);

	//Running on the array that was filled with the disk until it was filled
	for (int i = 0; i < index; i++)
	{
		//Checking whether the map already exists in the cache
		map = FindMapInfoByID(controlBlock->MapsSortedByID->root, array[i].id);

		//if not
		if (map == NULL)
		{
			//If the image is smaller than the cache
			if (array[i].size < SIZE_OF_CACHE)
				ReadNormalMap(array[i].id, array[i].size);
			//If the image is bigger than the cache
			else
				ReadMapBiggerThanCache(array[i].id, array[i].size);
		}
	}
}