#include"MasterCacheImages.h"
#include<errno.h>
#include <stdlib.h>
#include<stdio.h>


MasterCacheImg_cb_t* masterCacheImg_cb;

void MasterCacheImg_cb()
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
    return imgInfo;
}

void connectBetweenBothDatas(UnitNode_LRU_t* node, ImgInfo_t* imgInfo)
{
    //The pointers point to each other
    node->imgInfoPtr = imgInfo;
    imgInfo->unitNodePtr = node;
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
void throwExcptionToFile(ERRORS err)
{

}
