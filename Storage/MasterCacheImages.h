#pragma once

#define CACHE_SIZE 100

typedef enum 
{
	ALLOCATE_ERROR
}ERRORS;
 
typedef struct Point_s Point_t;
typedef struct ImgInfo_s ImgInfo_t;
typedef struct Stack_emptyPlaceInTheArray_s Stack_emptyPlaceInTheArray_t;
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

};


struct Stack_emptyPlaceInTheArray_s
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
	UnitNode_emptyPlaceInCache_t* emptyPlaceInCache;
	int imgArray[CACHE_SIZE];
	Stack_emptyPlaceInTheArray_t* emptyPlaceInTheArray;
	LinkedList_LRU_t* LRU;
};

Point_t createPoint(int x, int y);
UnitNode_LRU_t* createUnitNode_LRU(ImgInfo_t* imgInfo);
ImgInfo_t* createImgInfo(int imgId, int slaveId,Point_t TL,Point_t BR);
void throwExcptionToFile(ERRORS);
void connectBetweenBothDatas(UnitNode_LRU_t* node, ImgInfo_t* imgInfo);
LinkedList_LRU_t* initLinkedList();
void insertInToLinedList(UnitNode_LRU_t* node);
void MasterCacheImg_cb();
void moveToTheBeginning(UnitNode_LRU_t* node);
void initUnitNodeEmptyPlaceInCache();
void initQueueEmptyPlaceInCache();





