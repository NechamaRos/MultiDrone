#include"MasterCacheImages.h"
#include<errno.h>

UnitNode_LRU_t* createUnitNode_LRU(imgInfo_t imgInfo)
{
    UnitNode_LRU_t* node = (UnitNode_LRU_t*)malloce(sizeof(UnitNode_LRU_t));
    if (!node)
    {
        
    }
    return node;
}
