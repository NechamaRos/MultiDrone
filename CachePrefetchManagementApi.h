#pragma once

//function that get range and check if its in loading
bool Prefetch_IsRangeInLoading(Range_t rangeForSearch);

//function that get range to remove, find it and delete it from loadingArray
void Prefetch_RemoveRangeFromLoadingRanges(Range_t rangeForRemove);

//external function of insert new range 
void Prefetch_InsertNewRange(Range_t range);

void Prefetch_INIT(void);
