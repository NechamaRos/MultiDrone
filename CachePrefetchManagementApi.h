#pragma once
#include "CachePrefetchManagement.h"


//function that get range and check if its in loading
bool IsRangeInLoadingAPI(Range_t rangeForSearch);


//external function of insert new range 
void InsertNewRangeAPI(Range_t range);