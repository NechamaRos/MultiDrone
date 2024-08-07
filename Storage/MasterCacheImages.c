#include"MasterCacheImages.h"
#include<errno.h>
#include <stdlib.h>
#include <assert.h>
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
    memset(masterCacheImg_cb->cache, NULL, sizeof(masterCacheImg_cb->cache));

}

Point_t createPoint(int x, int y)
{
    Point_t p;
    p.x = x;
    p.y = y;
    return p;
}
ImgInfo_t* createImgInfo(int imgId, int slaveId, Point_t TL, Point_t BR)
{
    //allocate memory for ImgInfo_t
    ImgInfo_t* imgInfo = (ImgInfo_t*)malloc(sizeof(ImgInfo_t));
    //failed to assign
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
    int index = PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
    imgInfo->cachePtr = &(masterCacheImg_cb->cache[index]);
    return imgInfo;
}
UnitNode_LRU_t* createUnitNode_LRU(ImgInfo_t* imgInfo)
{
    //allocate memory for UnitNode_LRU_t
    UnitNode_LRU_t* node = (UnitNode_LRU_t*)malloc(sizeof(UnitNode_LRU_t));
    //failed to assign
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

void connectBetweenBothDatas(UnitNode_LRU_t* node, ImgInfo_t* imgInfo)
{
    //The pointers point to each other
    node->imgInfoPtr = imgInfo;
    imgInfo->unitNodePtr = node;
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

void PushEmptyPlaceInToStack(Stack_emptyPlace_t* stack, int index)
{
    int length = stack->length-1;
    //Updating the stack of imgArray that freed up space 
    stack->emptyPlaceInTheArray[length] = index;
    stack->length += 1;
}

int PopFirstEmptyPlaceInStack(Stack_emptyPlace_t* stack)
{
    stack->length -= 1;
    return stack->emptyPlaceInTheArray[stack->length];
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
    int indexInArrayCache = cachePtr - &(masterCacheImg_cb->cache[0]) ;
    //Updating the stack that freed up space in the cache 
    PushEmptyPlaceInToStack(masterCacheImg_cb->emptyPlaceInCache,indexInArrayCache);
    //update to null the cache memory
    cachePtr = NULL;
    masterCacheImg_cb->cache[indexInArrayCache] = NULL;
    
    
}
 
void removeFromImgArray(ImgInfo_t* imgInfo)
{
   int index = imgInfo - &(masterCacheImg_cb->imgArray[0]);
   removeFromCache(imgInfo->cachePtr);
   PushEmptyPlaceInToStack(masterCacheImg_cb->emptyPlaceInTheArray, index);
   //free the imgArray
   free(imgInfo);
   masterCacheImg_cb->imgArray[index] = NULL;
   return;
}
 
LinkedList_LRU_t* initLinkedList()
{
    ////allocate memory for LinkedList_LRU_t
    LinkedList_LRU_t* linedList_lru = (LinkedList_LRU_t*)malloc(sizeof(LinkedList_LRU_t));
    //failed to assign
    if (!linedList_lru)
    {
        errno = ALLOCATE_ERROR;
        throwExcptionToFile(ALLOCATE_ERROR);
    }
    //allocate memory for tail,head
    linedList_lru->head = (UnitNode_LRU_t*)malloc(sizeof(UnitNode_LRU_t));
    linedList_lru->tail = (UnitNode_LRU_t*)malloc(sizeof(UnitNode_LRU_t));
    //failed to assign
    if (!linedList_lru->head || !linedList_lru->tail)
    {
        errno = ALLOCATE_ERROR;
        throwExcptionToFile(ALLOCATE_ERROR);
    }
    else
    {
        linedList_lru->head->next = NULL;
        linedList_lru->tail->prev = NULL;
        linedList_lru->AmountOfLinks = 0;
        return linedList_lru;
    }
    
}

void insertInToLinedList(UnitNode_LRU_t* node)
{
    //if it the first node in the linkedList
    if (masterCacheImg_cb->LRU->AmountOfLinks == 0)
    {
        masterCacheImg_cb->LRU->head->next = node;
        masterCacheImg_cb->LRU->tail->prev = node;
        node->next = masterCacheImg_cb->LRU->tail;
        node->prev = masterCacheImg_cb->LRU->head;
    }
    //one or more links in the linkedList
    else
    {
        node->next = masterCacheImg_cb->LRU->head->next;
        masterCacheImg_cb->LRU->head->next = node;
        node->prev = masterCacheImg_cb->LRU->head;
        node->next->prev = node;
    }
    masterCacheImg_cb->LRU->AmountOfLinks += 1;

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
void removefromLinkedList()
{
    UnitNode_LRU_t* tmp;
    //loop that remove 10% of the cache
    for (int i = 0;i < CACHE_SIZE / 10;i++)
    {
        tmp = masterCacheImg_cb->LRU->tail->prev;
        masterCacheImg_cb->LRU->tail->prev = tmp->prev;
        tmp->prev->next = masterCacheImg_cb->LRU->tail;
        masterCacheImg_cb->LRU->AmountOfLinks -= 1;
        removeFromImgArray( tmp->imgInfoPtr);
        //freeing memory
        tmp->imgInfoPtr = NULL;
        free(tmp);
    }
}
char* stringError(ERRORS err)
{
    //switch the error to string
    switch (err)
    {
    case ALLOCATE_ERROR:
        return "allcocate error";
    default:
        return "";
        break;
    }
}

void throwExcptionToFile(ERRORS err)
{
    FILE file;
    //Attempt to open the file using fopen_s
    errno_t error = fopen_s(&file, ".\\masterMoce\\Exceptions.txt", "a");
    //file didt open
    if (error!=0)
    {
        //Write exception to cnsole
        printf("The file didn't open, try again ---------------Exception\n");
        return;
    }
    //convert err to string
    char* stringErr=stringError( err);
    //write error to file
    fprintf(&file, "%c\n",stringErr );
    //close file
    fclose(&file);
}