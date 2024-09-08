#pragma once
#define SIZE_OF_CACHE 200
#define SIZE_OF_QUEUE_ARRAY 10
#define SIZE_OF_RANGE_IN_QUEUE_ARRAY (SIZE_OF_CACHE/SIZE_OF_QUEUE_ARRAY)


typedef struct RangeInDataStorage_s {
	int location;
	int sizeOfBytes;
} RangeInDataStorage_t;


typedef struct Node_s {
	RangeInDataStorage_t data;
	struct Node_s* next;
} Node_t;


typedef struct MapInfo_s {
	int mapID;
	int mapSizeInBytes;
	Node_t* linkedList;
} MapInfo_t;

typedef struct AVLNode_s {
	void* data;
	struct AVLNode_s* left;
	struct AVLNode_s* right;
	int height;
} AVLNode_t;

typedef int (*CompareFunc)(const void*, const void*);

typedef struct AVLTree_s {
	AVLNode_t* root;
	CompareFunc compare;
} AVLTree_t;


// מבנה של תור
typedef struct QueueNode_s {
	MapInfo_t data;
	struct QueueNode_s* next;
} QueueNode_t;

typedef struct {
	QueueNode_t* front; // החזית של התור
	QueueNode_t* rear;  // הסוף של התור
} Queue_t;

typedef struct ValuesOfReadingMap_s {
	Node_t* linkedList;
	int BitsLeftToRead;
	int offset;
}ValuesOfReadingMap_t;

typedef struct controlBlock_s {
	int* cache[SIZE_OF_CACHE];
	Queue_t* queueArray[SIZE_OF_QUEUE_ARRAY];
	AVLTree_t* MapsSortedByID;
	AVLTree_t* emptyPlacesByLocation;
	AVLTree_t* emptyPlacesBySize;
	int EmptyPlaceInCache;
}controlBlock_t;


//linkedList

void printLinkedList(Node_t* head);

Node_t* createNode(int location, int sizeOfBytes);

void insertInTail(Node_t** head, int location, int sizeOfBytes);

void freeLinkedList(Node_t* head);

///-- - - - - - - - - - - - - avlTree - - - - - - - - - - - - 

int compareMapInfo(const void* a, const void* b);

int compareRangeByLocation(const void* a, const void* b);

int compareRangeBySize(const void* a, const void* b);

int height(AVLNode_t* node);

void updateHeight(AVLNode_t* node);

AVLNode_t* rightRotate(AVLNode_t* y);

AVLNode_t* leftRotate(AVLNode_t* x);

void addNode(AVLTree_t* tree, void* data);


//void inOrderTraversal(AVLNode_t* node, PrintFunc printFunc);

typedef void (*PrintFunc)(const void*);

void printAVLTree(AVLTree_t* tree, PrintFunc printFunc);

void printRangeInDataStorage(const void* data);

void printRangeInDataStorageTree(AVLTree_t* tree);

void printMapInfo(void* data);

void printMapInfoTree(AVLTree_t* tree);

int Abs(int x);

AVLNode_t* findMax(AVLNode_t* node);

AVLNode_t* findPrevNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc);

AVLNode_t* findNextNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc);
//we create function that wrap the function of find prev, and function of find next, get it and compare it if is complete long hole, and merge it to one node in tree....

void mergeNodes(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc);

//AVLNode_t* deleteRangeNode(AVLNode_t* root, int locationToDelete, CompareFunc compare);

AVLNode_t* FindMapInfoByID(AVLNode_t* root, int mapID);

AVLNode_t* FindRangeByLocation(AVLNode_t* root, int location);

void UpdateNodeInRangeBySize(RangeInDataStorage_t* nodeToUpdate, int updatedSize);

void UpdateNodeInRangeByLocation(AVLNode_t* nodeToUpdate, int updatedSize);

AVLNode_t* RecursiveDeleteNodeFromEmptyPlacesBySize(AVLNode_t* root, RangeInDataStorage_t* item);

AVLNode_t* RecursiveDeleteNodeFromEmptyPlacesByLocation(AVLNode_t* root, int objectToSearchBy);

AVLNode_t* RucursiveDeleteMapFromMapsSortedByID(AVLNode_t* root, int mapID);

void deleteNodeFromEmptyPlacesByLocation(int location);

void deleteNodeFromEmptyPlacesBySize(RangeInDataStorage_t* item);

void deleteNodeFromMapsSortedByID(int id);


void printTree(AVLNode_t* root);

/////////////////////////////////- - - - queue - - - -/////////////////////////////////////////

Queue_t* createQueue();

// פונקציה להוספת פריט לתור (enqueue)
void enqueue(Queue_t* queue, MapInfo_t mapInfo);

MapInfo_t* dequeue(Queue_t* queue);

int isQueueEmpty(Queue_t* queue);

void freeQueue(Queue_t* queue);

int calculateIndexInQueueArray(int size);

void initControlBlock();

MapInfo_t* removeMaxMapFromQueueArray();


/////

void MakesRoomForCurrentIncome(int size);

void removeMapFromCache(int id);

ValuesOfReadingMap_t ReadPieceOfMap(Node_t* linkedListOfLocations, int size, int offset, int id);

void fetchFromDisk(int id, int offset, int location, int size);

void insertToQueueArray(MapInfo_t* mapInfo);

void ReadNormalMap(int id, int size);

