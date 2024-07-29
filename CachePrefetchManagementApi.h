#pragma once
#include "CachePrefetchManagement.h"


//function that get range and check if its in loading
bool IsRangeInLoadingAPI(Range_t rangeForSearch);

//function that get range to remove, find it and delete it from loadingArray
void RemoveRangeFromLoadingRangesAPI(Range_t rangeForRemove);

//external function of insert new range 
void InsertNewRangeAPI(Range_t range);