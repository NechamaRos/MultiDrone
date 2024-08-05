#pragma once
#define CACHE_SIZE 100
 

typedef struct point_s
{
	int x;
	int y;

}point_t;


typedef struct imgInfo_s
{
	int imgId;
	int slaveId;
	point_t TL;
	point_t BR;
	struct unitNode_LRU_s* unitNodePtr;

}imgInfo_t;


typedef struct stack_emptyPlaceInTheArray_s
{
	int emptyPlaceInTheArray[CACHE_SIZE];
	int length;

}stack_emptyPlaceInTheArray_t;



typedef struct UnitNode_LRU_s
{
	struct UnitNode_LRU_s* next;
	struct UnitNode_LRU_s* prev;
	imgInfo_t* imgInfoPtr;

}UnitNode_LRU_t;



typedef struct linkedList_LRU_s
{
	UnitNode_LRU_t* head;
	UnitNode_LRU_t* tail;

}linkedList_LRU_t;



typedef struct UnitNode_emptyPlaceInCache_s
{
  int emptyPlaceInCache;
  struct UnitNode_emptyPlaceInCache_s* next;

}UnitNode_emptyPlaceInCache_t;



typedef struct queue_emptyPlaceInCache_s
{
	UnitNode_emptyPlaceInCache_t* head;
	UnitNode_emptyPlaceInCache_t* tail;
} queue_emptyPlaceInCache_t;


typedef struct MasterCacheImg_cb_s
{
	int cache[CACHE_SIZE];
	queue_emptyPlaceInCache_t* emptyPlaceInCache;
	int imgArray[CACHE_SIZE];
	stack_emptyPlaceInTheArray_t* emptyPlaceInTheArray;
	linkedList_LRU_t* LRU;
}MasterCacheImg_cb_t;

UnitNode_LRU_t* createUnitNode_LRU(imgInfo_t imgInfo);



