#pragma once
#define SIZE_OF_CACHE 200
#define SIZE_OF_QUEUE_ARRAY 10
#define SIZE_OF_RANGE_IN_QUEUE_ARRAY (SIZE_OF_CACHE/SIZE_OF_QUEUE_ARRAY)


//object of Range In Data Storage
typedef struct RangeInDataStorage_s {
	int location;
	int sizeOfBytes;
} RangeInDataStorage_t;

//A node object, contains a data of type RangeInDataStorage_t
typedef struct Node_s {
	RangeInDataStorage_t data;
	struct Node_s* next;
} Node_t;

//A map object, contains a linkedList of the data-locations
typedef struct MapInfo_s {
	int mapID;
	int mapSizeInBytes;
	Node_t* linkedList;
} MapInfo_t;

//A generic type of a tree node
typedef struct AVLNode_s {
	void* data;
	struct AVLNode_s* left;
	struct AVLNode_s* right;
	int height;
} AVLNode_t;

//a generic type of compare function
typedef int (*CompareFunc)(const void*, const void*);

//a generic type of print item function
typedef void (*PrintFunc)(const void*);

//a generic type of AVLTree
typedef struct AVLTree_s {
	AVLNode_t* root;
	CompareFunc compare;
} AVLTree_t;

//A node object, contains a data of type MapInfo_t
typedef struct QueueNode_s {
	MapInfo_t data;
	struct QueueNode_s* next;
} QueueNode_t;

//A Queue object
typedef struct {
	QueueNode_t* front; 
	QueueNode_t* rear;  
} Queue_t;

//An object containing information about the state of reading the map
typedef struct ValuesOfReadingMap_s {
	Node_t* linkedList;
	int BitsLeftToRead;
	int offset;
}ValuesOfReadingMap_t;

//controlBlock
typedef struct controlBlock_s {
	int* cache[SIZE_OF_CACHE];
	Queue_t* QueuesArray[SIZE_OF_QUEUE_ARRAY];
	AVLTree_t* MapsSortedByID;
	AVLTree_t* emptyPlacesByLocation;
	AVLTree_t* emptyPlacesBySize;
	int EmptyPlaceInCache;
}controlBlock_t;

//struct that representing a point in a plane
typedef struct Point_s
{
	int x;
	int y;
}Point_t;

//object of map, when I ask it from disk
typedef struct map_s
{
	int id;
	int size;
}map_t;


////////////////////////////////////////////////////////////////LinkedList functions

void PrintLinkedList(Node_t* head);

//create new node in linkedList
Node_t* CreateRangeInDataStorageNode(int location, int sizeOfBytes);

//insert in tail of linkedList
void InsertInTailOfLinkedList(Node_t** head, int location, int sizeOfBytes);

//free linkedList and remove it to an EmptyPlaces AVLTrees
void FreeLinkedList(Node_t* head);

///////////////////////////////////AVLTrees functions

//Compare by mapID
int CompareMapInfo(const void* a, const void* b);

//Compare by range location
int CompareRangeByLocation(const void* a, const void* b);

//Compare by range sizeOfBytes
int CompareRangeBySize(const void* a, const void* b);

//Get Height
int Height(AVLNode_t* node);

//Update Height
void UpdateHeight(AVLNode_t* node);

AVLNode_t* RightRotate(AVLNode_t* y);

AVLNode_t* LeftRotate(AVLNode_t* x);

//Function of insertion new node to avlTree, calls insertNode
void AddNodeToAvlTree(AVLTree_t* tree, void* data);

//Recursive function of insertion new node to AVLTree
AVLNode_t* InsertNode(AVLNode_t* node, void* data, CompareFunc compare);

//print functions
void PrintAVLTree(AVLTree_t* tree, PrintFunc printFunc);

void PrintRangeInDataStorage(const void* data);

void PrintRangeInDataStorageTree(AVLTree_t* tree);

void PrintMapInfo(void* data);

void PrintMapInfoTree(AVLTree_t* tree);

//return absolute value
int Abs(int x);

//Find the maximal value in tree
AVLNode_t* FindMax(AVLNode_t* node);

//Find the minimal value in tree
AVLNode_t* FindMin(AVLNode_t* node);

//AVLNode_t* FindPrevNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc);

//AVLNode_t* FindNextNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc);

//Run on maps tree and free each node memory
void FreeMapInfoTree(AVLNode_t* root);

//Find map in the tree by id
AVLNode_t* FindMapInfoByID(AVLNode_t* root, int mapID);

//Find range node in the tree sorted by location
AVLNode_t* FindRangeByLocation(AVLNode_t* root, int location);

//Update range when I filled only part of the range
void UpdateNodeInRangeBySize(RangeInDataStorage_t* nodeToUpdate, int updatedSize);

//Update range when I filled only part of the range
void UpdateNodeInRangeByLocation(AVLNode_t* nodeToUpdate, int updatedSize);

//Recursive function of Delete EmptyPlacesBySize tree
AVLNode_t* RecursiveDeleteNodeFromEmptyPlacesBySize(AVLNode_t* root, RangeInDataStorage_t* item);

//Recursive function of Delete EmptyPlacesByLocation tree
AVLNode_t* RecursiveDeleteNodeFromEmptyPlacesByLocation(AVLNode_t* root, int objectToSearchBy);

//Recursive function of Delete map from MapsSortedByID tree 
AVLNode_t* RecursiveDeleteMapFromMapsSortedByID(AVLNode_t* root, int mapID);

//Wrap function of Delete from emptyPlacesByLocation tree
void DeleteNodeFromEmptyPlacesByLocation(int location);

//Wrap function of Delete from emptyPlacesBySize tree
void DeleteNodeFromEmptyPlacesBySize(RangeInDataStorage_t* item);

//Wrap function of Delete from MapsSortedByID tree
void DeleteNodeFromMapsSortedByID(int id);

///////////////////////////////////////////////Queue functions

//Create new queue
Queue_t* CreateQueue();

//Insert to queue
void Enqueue(Queue_t* queue, MapInfo_t mapInfo);

//Remove from queue
MapInfo_t* Dequeue(Queue_t* queue);

//Check if queue is empty
int IsQueueEmpty(Queue_t* queue);

//Free queue's nodes
void FreeQueue(Queue_t* queue);

//Calculate the index of queue to insert new map by its size
int CalculateIndexInQueuesArray(int size);

//Remove the bigger map inserted recently
MapInfo_t* RemoveMaxMapFromQueuesArray();

//Insert to QueuesArray function
void InsertToQueuesArray(MapInfo_t* mapInfo);

//////////////////////////////////////////////////Central functions

//Function of removing map from cache by id
void RemoveMapFromCache(int id);

//Function of reading a part from map
ValuesOfReadingMap_t ReadPieceOfMap(Node_t* linkedListOfLocations, int size, int offset, int id);

//Dummy function of request to read from disk
void FetchFromDisk(int id, int offset, int location, int size);

//Function of reading map smaller than cache size
void ReadNormalMap(int id, int size);

//Function of reading map bigger than cache size
void ReadMapBiggerThanCache(int id, int mapSize);

//Function of initial
void InitControlBlock();

//Function of reset the data structures
void ResetCache();

//Wrap function that makes sure there is enough space
void MakesRoomForCurrentIncome(int size);

//Function of clearance is sufficient for the income
void OverrideInternal(int size);

//A function that accepts a request for a map according to 2 points longitude and latitude
void LoadMapsByBR_TL(Point_t br, Point_t tl);
