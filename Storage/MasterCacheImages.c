#include"MasterCacheImages.h"
#include<errno.h>
#include <stdlib.h>
#include<stdio.h>


MasterCacheImg_cb_t* masterCacheImg_cb;

void initMasterCacheImg_cb()
{
    //allocate memory for MasterCacheImg_cb_t
    masterCacheImg_cb = (MasterCacheImg_cb_t*)malloc(sizeof(MasterCacheImg_cb_t));
    //failed to assign
    if (!masterCacheImg_cb)
    {
        errno = ALLOCATE_ERROR;
        throwExcptionToFile(ALLOCATE_ERROR);
    }
    masterCacheImg_cb->LRU = initLinkedList();
    masterCacheImg_cb->emptyPlaceInCache = initStuck();
    masterCacheImg_cb->emptyPlaceInTheArray = initStuck();
    memset(masterCacheImg_cb->imgArray, NULL, sizeof(masterCacheImg_cb->imgArray));
}

Point_t createPoint(int x, int y)
{
    Point_t p;
    p.x = x;
    p.y = y;
    return p;
}

UnitNode_LRU_t* createUnitNode_LRU(ImgInfo_t* imgInfo)
{
    UnitNode_LRU_t* node = (UnitNode_LRU_t*)malloc(sizeof(UnitNode_LRU_t));
    if (!node)
    {
        errno = ALLOCATE_ERROR;
        throwExcptionToFile(ALLOCATE_ERROR);
    }
    node->next = NULL;
    node->prev = NULL;
    connectBetweenBothDatas(node,imgInfo);
    return node;
}

ImgInfo_t* createImgInfo(int imgId, int slaveId, Point_t TL, Point_t BR)
{
    ImgInfo_t* imgInfo = (ImgInfo_t*)malloc(sizeof(ImgInfo_t));
    if (!imgInfo)
    {
        errno = ALLOCATE_ERROR;
        throwExcptionToFile(ALLOCATE_ERROR);
    }
    imgInfo->imgId = imgId;
    imgInfo->slaveId = slaveId;
    imgInfo->TL = TL;
    imgInfo->BR = BR;
    imgInfo->unitNodePtr = NULL;
    return imgInfo;
}

void connectBetweenBothDatas(UnitNode_LRU_t* node, ImgInfo_t* imgInfo)
{
    node->imgInfoPtr = imgInfo;
    imgInfo->unitNodePtr = node;
}

}
void moveToTheBeginning(UnitNode_LRU_t* node)
{
    //there is more than one link in the linkedList
    if (masterCacheImg_cb->LRU->AmountOfLinks > 1)
    {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        node->prev = masterCacheImg_cb->LRU->head;
        node->next = masterCacheImg_cb->LRU->head->next;
        masterCacheImg_cb->LRU->head->next->prev = node;
        masterCacheImg_cb->LRU->head->next = node;
    }
}
Stack_emptyPlace_t* initStuck()
{
    Stack_emptyPlace_t* stack = (Stack_emptyPlace_t*)malloc(sizeof(Stack_emptyPlace_t));
    //failed to assign
    if (!stack)
    {
        errno = ALLOCATE_ERROR;
        throwExcptionToFile(ALLOCATE_ERROR);
    }
    //Initialize all the places to be free
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        stack->emptyPlaceInTheArray[i] = CACHE_SIZE-i-1;
    }
    stack->length = CACHE_SIZE;
    return stack;
}

void insertTocache(int* imgData)
{
    int index = CACHE_SIZE - masterCacheImg_cb->emptyPlaceInCache->length;
    masterCacheImg_cb->cache[index] = imgData;
    int indexInStackOfCache = masterCacheImg_cb->emptyPlaceInCache->length;
    masterCacheImg_cb->emptyPlaceInCache->emptyPlaceInTheArray[indexInStackOfCache]=index;
}

void removeFromCache(int* cachePtr)
{
    //Calculation of freed cache memory space
    int indexInArrayCache = &(masterCacheImg_cb->cache) - &cachePtr;
    //Updating the stack that freed up space in the cache 
    addEmptyPlaceInCacheStack(indexInArrayCache);
    //free the cache memory
    free(cachePtr);
    masterCacheImg_cb->cache[indexInArrayCache] = NULL;
    
}
void addEmptyPlaceInCacheStack(int index)
{
    int length = masterCacheImg_cb->emptyPlaceInCache->length;
    //Updating the stack of cache that freed up space 
    masterCacheImg_cb->emptyPlaceInCache->emptyPlaceInTheArray[length] = index;
    masterCacheImg_cb->emptyPlaceInCache->length += 1;

}
void addEmptyPlaceInImgArrayStack(int index)
{
    int length = masterCacheImg_cb->emptyPlaceInTheArray->length;
    //Updating the stack of imgArray that freed up space 
    masterCacheImg_cb->emptyPlaceInTheArray->emptyPlaceInTheArray[length] = index;
    masterCacheImg_cb->emptyPlaceInTheArray->length += 1;
}
removefromImgArray(ImgInfo_t* imgInfo)
{
   int index= &(masterCacheImg_cb->imgArray) - &(imgInfo);
   removeFromCache(imgInfo->cachePtr);
   addEmptyPlaceInImgArrayStack(index);
   //free the imgArray
   free(imgInfo);
   masterCacheImg_cb->imgArray[index] = NULL;
}

void throwExcptionToFile(ERRORS err)
{

}
