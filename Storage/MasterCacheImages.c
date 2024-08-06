#include"MasterCacheImages.h"
#include<errno.h>
#include <stdlib.h>
#include<stdio.h>

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

void throwExcptionToFile(ERRORS err)
{

}
