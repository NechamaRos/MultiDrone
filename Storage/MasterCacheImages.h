#pragma once

#define CACHE_SIZE 100

typedef enum 
{
	ALLOCATE_ERROR
}ERRORS;
 
typedef struct Point_s Point_t;
typedef struct ImgInfo_s ImgInfo_t;
typedef struct Stack_emptyPlace_s Stack_emptyPlace_t;
typedef struct UnitNode_LRU_s UnitNode_LRU_t;
typedef struct LinkedList_LRU_s LinkedList_LRU_t;
typedef struct UnitNode_emptyPlaceInCache_s UnitNode_emptyPlaceInCache_t;
typedef struct Queue_emptyPlaceInCache_s Queue_emptyPlaceInCache_t;
typedef struct MasterCacheImg_cb_s MasterCacheImg_cb_t;

struct Point_s
{
	int x;
	int y;

};

struct ImgInfo_s
{
	int imgId;
	int slaveId;
	Point_t TL;
	Point_t BR;
	UnitNode_LRU_t* unitNodePtr;
	int* cachePtr;
};


struct Stack_emptyPlace_s
{
	int emptyPlaceInTheArray[CACHE_SIZE];
	int length;
};


struct UnitNode_LRU_s
{
    UnitNode_LRU_t* next;
    UnitNode_LRU_t* prev;
	ImgInfo_t* imgInfoPtr;

};

 struct LinkedList_LRU_s
{
	UnitNode_LRU_t* head;
	UnitNode_LRU_t* tail;
	int AmountOfLinks;
};

struct UnitNode_emptyPlaceInCache_s
{
  int emptyPlaceInCache;
  UnitNode_emptyPlaceInCache_t* next;

};

struct queue_emptyPlaceInCache_s
{
	UnitNode_emptyPlaceInCache_t* head;
	UnitNode_emptyPlaceInCache_t* tail;
} ;

struct MasterCacheImg_cb_s
{
	int cache[CACHE_SIZE];
	Stack_emptyPlace_t* emptyPlaceInCache;
	ImgInfo_t* imgArray[CACHE_SIZE];
	Stack_emptyPlace_t* emptyPlaceInTheArray;
	LinkedList_LRU_t* LRU;
};


//info
Point_t createPoint(int x, int y);
ImgInfo_t* createImgInfo(int imgId, int slaveId,Point_t TL,Point_t BR);

// cb
void initMasterCacheImg_cb();
void freeMasterCacheImg_cb();
void removeAllData();


//linkedList
UnitNode_LRU_t* createUnitNode_LRU(ImgInfo_t* imgInfo);
void connectBetweenBothDatas(UnitNode_LRU_t* node, ImgInfo_t* imgInfo);
LinkedList_LRU_t* initLinkedList();
void insertInToLinedList(UnitNode_LRU_t* node);
void moveToTheBeginning(UnitNode_LRU_t* node);
void removefromLinkedList();
void removeTenPercentFromCache();

//stack
Stack_emptyPlace_t* initStuck();
void PushEmptyPlaceInToStack(Stack_emptyPlace_t* stack,int index);
int PopFirstEmptyPlaceInStack(Stack_emptyPlace_t* stack);

//cache
void insertTocache(int* imgData);
void removeFromCache(int* cachePtr);

//imgArray
void removeFromImgArray(ImgInfo_t* imgInfoPtr);

//help func
char* stringError(ERRORS err);
void throwExcptionToFile(ERRORS);



