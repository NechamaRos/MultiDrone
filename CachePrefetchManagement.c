#include "CachePrefetchManagement.h"
#include "CachePrefetchManagementApi.h"
#include <stdio.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS // Required for localtime_s on some compilers

#pragma region Init variables

SequenceCollectionCtrlBlk_t* prefetchSeqCollection_CB;

FILE* ErrorFile;

void Prefetch_INIT(void)
{
	char filename[] = "ErrorLog.txt";

	// Open the file for writing without overwriting existing content
	if (fopen_s(&ErrorFile, filename, "a") != 0) {
		perror("Unable to open the file for writing");
		exit(1);
	}

	prefetchSeqCollection_CB = (SequenceCollectionCtrlBlk_t*)malloc(sizeof(SequenceCollectionCtrlBlk_t));

	if (prefetchSeqCollection_CB == NULL) {
		fprintf(ErrorFile, "Failed to allocate memory for Sequence Pointer - - - ");
		PrintCurrentTimeToErrorFile();
		fclose(ErrorFile);
		perror("Failed to allocate memory for Sequence Pointer");
		exit(1);
	}

	prefetchSeqCollection_CB->singleRangesArray.index = 0;


	//initial the SeqRangeInfoArray with garbage values
	for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++)
	{
		prefetchSeqCollection_CB->seqRangeInfoArray[i] = (SeqRangeInfo_t*)malloc(sizeof(SeqRangeInfo_t));

		if (prefetchSeqCollection_CB->seqRangeInfoArray[i] == NULL) {
			fprintf(ErrorFile, "Failed to allocate memory for prefetchSeqCollection_CB->seqRangeInfoArray - - - ");
			PrintCurrentTimeToErrorFile();
			fclose(ErrorFile);
			perror("Failed to allocate memory for prefetchSeqCollection_CB->seqRangeInfoArray");
			exit(1);
		}

		prefetchSeqCollection_CB->seqRangeInfoArray[i]->counter = -1;

		prefetchSeqCollection_CB->seqRangeInfoArray[i]->counterUse = -1;

		prefetchSeqCollection_CB->seqRangeInfoArray[i]->dir = IN_VALID_DIR;

		prefetchSeqCollection_CB->seqRangeInfoArray[i]->lastInsertedRange = GetDefaultRange();

		prefetchSeqCollection_CB->seqRangeInfoArray[i]->nextExpectedRange = GetDefaultRange();

		prefetchSeqCollection_CB->singleRangesArray.array[i] = GetDefaultRange();

		prefetchSeqCollection_CB->RangesInLoadingArray[i] = GetDefaultRange();

	}
}

#pragma endregion

#pragma region Range Functions

bool CheckValidationOfRange(Range_t range)
{
	if (!(IsEqualRanges(GetDefaultRange(), range)))
		if (range.topLeft.x + SUPPORTED_RANGE_WIDTH == range.bottomRight.x && range.topLeft.y - SUPPORTED_RANGE_LENGTH == range.bottomRight.y)
			return true;
	return false;
}
//get default range for delete range
Range_t GetDefaultRange(void) {
	Range_t defRange = INIT_RANGE_VALUE;
	return defRange;
}

//check if the 2 ranges are equal
bool IsEqualRanges(Range_t me, Range_t other)
{
	return me.bottomRight.x == other.bottomRight.x &&
		me.bottomRight.y == other.bottomRight.y &&
		me.topLeft.x == other.topLeft.x &&
		me.topLeft.y == other.topLeft.y;
}

//function to get the next expected range of sequence by the sequence's direction
Range_t GetNextRangeByDirection(Range_t range, Range_Direction_t dir)
{
	int bottomRightX = range.bottomRight.x;
	int bottomRightY = range.bottomRight.y;
	int topLeftX = range.topLeft.x;
	int topLeftY = range.topLeft.y;

	switch (dir) {
	case UP:
		bottomRightY = range.bottomRight.y + SUPPORTED_RANGE_LENGTH;//up the bottomRightY in SUPPORTED_RANGE_LENGTH
		topLeftY = range.topLeft.y + SUPPORTED_RANGE_LENGTH;//up the topLeftY in SUPPORTED_RANGE_LENGTH
		break;
	case DOWN:
		bottomRightY = range.bottomRight.y - SUPPORTED_RANGE_LENGTH;//down bottomRightY in SUPPORTED_RANGE_LENGTH
		topLeftY = range.topLeft.y - SUPPORTED_RANGE_LENGTH;//down topLeftY in SUPPORTED_RANGE_LENGTH
		break;
	case LEFT:
		bottomRightX = range.bottomRight.x - SUPPORTED_RANGE_WIDTH;//down the bottomRightX in SUPPORTED_RANGE_WIDTH
		topLeftX = range.topLeft.x - SUPPORTED_RANGE_WIDTH;//down the topLeftX in SUPPORTED_RANGE_WIDTH
		break;
	case RIGHT:
		bottomRightX = range.bottomRight.x + SUPPORTED_RANGE_WIDTH;//up the bottomRightX in SUPPORTED_RANGE_WIDTH
		topLeftX = range.topLeft.x + SUPPORTED_RANGE_WIDTH;//up the topLeftX in SUPPORTED_RANGE_WIDTH
		break;
	}
	//set the bottomRight Point_t
	Point_t myBottomRight = { bottomRightX,bottomRightY };

	//set the topLeft Point
	Point_t myTopLeft = { topLeftX , topLeftY };

	Range_t nextRange = { myBottomRight,myTopLeft };//set the new Range to return
	//declare the variables to the start...
	return nextRange;
}

//return the direction that the range continue the single range and IN_VALID_DIR if it not continue
Range_Direction_t IsRangeCompleteSingleRangeToSequence(Range_t range, Range_t singleRange)
{
	//same longitude line 
	if (range.topLeft.x == singleRange.topLeft.x) {
		//if the y of topLeft of current  Range_t same my bottomRight y - it down direction
		if (range.topLeft.y == singleRange.bottomRight.y)
			return DOWN;

		//if the y of bottom right of current  Range_t same my topLeft y - it up direction
		if (range.bottomRight.y == singleRange.topLeft.y)
			return UP;
	}

	else if (range.topLeft.y == singleRange.topLeft.y) {

		//if the x of topLeft of current  Range_t same  myBottomRight x - it right direction
		if (range.topLeft.x == singleRange.bottomRight.x)
			return RIGHT;

		//if the x of topLeft of current  Range_t same  myBottomRight x - it down direction

		if (range.bottomRight.x == singleRange.topLeft.x)
			return LEFT;
	}
	//if the current  Range_t doesn't continue according to any direction
	return 	IN_VALID_DIR;
}

//for check if range in loading
bool IsRangeContainedInOtherRange(Range_t currentSmallRange, Range_t containRange)
{
	if ((containRange.topLeft.x <= currentSmallRange.topLeft.x) &&
		(containRange.topLeft.y >= currentSmallRange.topLeft.y) &&
		(containRange.bottomRight.x >= currentSmallRange.bottomRight.x) &&
		(containRange.bottomRight.y <= currentSmallRange.bottomRight.y)) {

		return true; // True,  Range_t is contained
	}
	return false; // False,  Range_t is not contained
}

#pragma endregion

#pragma region sequence Collection Functions

// Function to check if a given range continues an existing sequence in seqRangeInfoArray.
int IsRangeContinueSequence(Range_t range)
{
	int index = -1;

	// Iterate through prefetchSeqCollection_CB to find a match for range's NextExpectedRange.
	for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++)
	{

		if (IsEqualRanges(prefetchSeqCollection_CB->seqRangeInfoArray[i]->nextExpectedRange, range)) {

			// Update sequence details if range continues the sequence.
			prefetchSeqCollection_CB->seqRangeInfoArray[i]->counterUse = 0;
			prefetchSeqCollection_CB->seqRangeInfoArray[i]->lastInsertedRange = range;
			prefetchSeqCollection_CB->seqRangeInfoArray[i]->nextExpectedRange = GetNextRangeByDirection(range, prefetchSeqCollection_CB->seqRangeInfoArray[i]->dir);
			prefetchSeqCollection_CB->seqRangeInfoArray[i]->counter++;
			index = i;
			break;
		}
	}
	return index;
}

// Function to check if a given range continues an existing range in singleRangeArray to sequence.
//if y create the new sequence
int IsRangeCreateNewSequence(Range_t range)
{
	int index = -1;
	SeqRangeInfo_t* newSeq;
	newSeq = (SeqRangeInfo_t*)malloc(sizeof(SeqRangeInfo_t));
	Range_Direction_t direction = IN_VALID_DIR;
	// Function to check if a given range creates a new sequence by comparing it with ranges in singleRangesArray.
	for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++)
	{
		// Check if range creates a new sequence with singleRangesArray[i].
		if (!IsEqualRanges(prefetchSeqCollection_CB->singleRangesArray.array[i], GetDefaultRange()))
		{
			direction = IsRangeCompleteSingleRangeToSequence(range, prefetchSeqCollection_CB->singleRangesArray.array[i]);
		}
		if (direction != IN_VALID_DIR)
		{
			// Create new sequence and update prefetchSeqCollection_CB; remove corresponding range from singleRangesArray.

			newSeq->counter = INIT_COUNTER;
			newSeq->counterUse = 0;
			newSeq->dir = direction;
			newSeq->lastInsertedRange = range;
			newSeq->nextExpectedRange = GetNextRangeByDirection(range, direction);

			index = GetLastUsedSequence();
			prefetchSeqCollection_CB->seqRangeInfoArray[index] = newSeq;

			prefetchSeqCollection_CB->singleRangesArray.array[i] = GetDefaultRange();

			direction = IN_VALID_DIR;
			return index;
		}
	}
	return index;
}

//for LRU
int GetLastUsedSequence(void)
{
	int max = -1;
	int index = -1;
	// Iterates through prefetchSeqCollection_CB_t to find the sequence with the highest CounterUse.
	for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++) {
		if (prefetchSeqCollection_CB->seqRangeInfoArray[i]->counterUse == -1) {
			return i;
		}
		if (prefetchSeqCollection_CB->seqRangeInfoArray[i]->counterUse > max) {
			index = i;
			max = prefetchSeqCollection_CB->seqRangeInfoArray[i]->counterUse;
		}
	}
	return index;
}

void Prefetch_InsertNewRange(Range_t range)
{
	if (!CheckValidationOfRange(range))
	{
		fprintf(ErrorFile, "The inserted range is not valid - - - ");
		PrintCurrentTimeToErrorFile();
		perror("The inserted range is not valid\n");
	}
	else
		InsertNewRange(range);
}

//insert new range to the prefetchSeqCollection_CB and return if the range is single/create new sequence / continue an existing sequence
INSERT_ACTION_TYPE_t InsertNewRange(Range_t range)//return enum of state insert
{

	//if the range continue sequence, insert it to sequence collection
	int index = IsRangeContinueSequence(range);

	// If range continues an existing sequence, call ReadingAhead for asking the next range.
	if (index != -1) {
		IncreaseAllLRUCountersExceptSpecificCounter(index);
		ReadingAhead(prefetchSeqCollection_CB->seqRangeInfoArray[index]);
		return CONTINUE_SEQ;
	}

	// Check if range creates a new sequence
	// if true, insert it to sequence collection and remove the prev range from the single ranges collection

	index = IsRangeCreateNewSequence(range);
	if (index != -1) {
		IncreaseAllLRUCountersExceptSpecificCounter(index);
		return CREATE_SEQ;
	}
	// if not, insert it into singleRangesArray
	prefetchSeqCollection_CB->singleRangesArray.array[prefetchSeqCollection_CB->singleRangesArray.index] = range;
	prefetchSeqCollection_CB->singleRangesArray.index = (prefetchSeqCollection_CB->singleRangesArray.index + 1) % MAX_SUPPORTED_PARALLEL_RANGE;
	return INSERT_TO_SINGLE_RANGES_ARRAY;
}

void IncreaseAllLRUCountersExceptSpecificCounter(int index) {
	//run over on seqRangeInfoArray
	for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++) {
		//if seqRangeInfoArray[i] is not the seq in index of seqRangeInfoArray that continue now
		//and if it is not a null seqRangeInfoArray, update the counterUse up
		if (i != index && !IsEqualRanges(prefetchSeqCollection_CB->seqRangeInfoArray[i]->lastInsertedRange, GetDefaultRange())) {
			prefetchSeqCollection_CB->seqRangeInfoArray[i]->counterUse++;
		}
	}
}

#pragma endregion


//read range/s ahead if its needed
void ReadingAhead(SeqRangeInfo_t* seq)
{
	Range_t r;
	Range_Direction_t dir = seq->dir;
	//if it is a new enough sequence, read double range
	if (seq->counter == SIZE_OF_ENOUGH_SEQ)
	{
		r = seq->lastInsertedRange;

		for (size_t i = 0; i < COUNT_OF_RANGE_TO_READING_AHEAD; i++)
		{
			// Read two ranges forward and handle caching operations from disk.
			r = GetNextRangeByDirection(r, seq->dir);
			if (dir == DOWN)
			{
				if (r.bottomRight.y < 0)
					r.bottomRight.y = 0;
			}
			else if (dir == LEFT)
			{
				if (r.topLeft.x < 0)
					r.topLeft.x = 0;
			}
			AddRangeToLoadingRangeArray(r);
			FetchFromDisk(r);
		}

		r = GetNextRangeByDirection(r, seq->dir);
		//send req to cache to load this range
		return;
	}

	else
	{
		r = seq->lastInsertedRange;
		// Read two ranges forward and handle caching operations from disk.
		for (int i = 0; i < COUNT_OF_RANGE_TO_READING_AHEAD; i++)
		{
			r = GetNextRangeByDirection(r, seq->dir);
		}
		//send req to cache to load this range

		//if the range need to reading ahead is not valid, read until the valid
		if (dir == DOWN) {
			if (r.bottomRight.y < 0)
				r.bottomRight.y = 0;
		}

		else if (dir == LEFT) {
			if (r.topLeft.x < 0)
				r.topLeft.x = 0;
		}
		AddRangeToLoadingRangeArray(r);
		FetchFromDisk(r);
	}
}

#pragma region RangesInLoadingArray Functions

//check if range in loading
bool Prefetch_IsRangeInLoading(Range_t rangeForSearch)
{
	if ((IsEqualRanges(GetDefaultRange(), rangeForSearch)))
	{
		fprintf(ErrorFile, "The range to found is not valid - - - ");
		PrintCurrentTimeToErrorFile();
		perror("The range to found is not valid\n");
	}
	else
	{
		for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++)
		{
			if (IsRangeContainedInOtherRange(rangeForSearch, prefetchSeqCollection_CB->RangesInLoadingArray[i]))
				return true;
		}
	}
	return false;
}

//remove loaded range from loadingArray
void Prefetch_RemoveRangeFromLoadingRanges(Range_t rangeForRemove)
{
	int index = FindRangeInLoadingRangesArray(rangeForRemove);
	if (index != -1)
		prefetchSeqCollection_CB->RangesInLoadingArray[index] = GetDefaultRange();
}

//get the next empty index in loadingArray
int GetNextEmptyIndexOfRangesInLoadingArray(void)
{
	int index = -1, i = 0;
	while (index == -1 && i <= MAX_SUPPORTED_PARALLEL_RANGE)
	{
		if (IsEqualRanges(prefetchSeqCollection_CB->RangesInLoadingArray[i], GetDefaultRange()))
			index = i;
		else
			i++;
	}
	return index == -1 ? 0 : index;
}

//find the index of range
int FindRangeInLoadingRangesArray(Range_t rangeForRemove)
{
	int index = -1, i = 0;
	while (index == -1 && i <= MAX_SUPPORTED_PARALLEL_RANGE)
	{
		if (IsEqualRanges(prefetchSeqCollection_CB->RangesInLoadingArray[i], rangeForRemove))
			index = i;
		else
			i++;
	}
	return index;
}

//add range to loadingArray
void AddRangeToLoadingRangeArray(Range_t rangeForAdd)
{
	prefetchSeqCollection_CB->RangesInLoadingArray[GetNextEmptyIndexOfRangesInLoadingArray()] = rangeForAdd;
}
#pragma endregion

void PrintCurrentTimeToErrorFile(void)
{
	time_t now;            // Variable to hold the current time in seconds since Epoch
	struct tm local_time;  // Structure to store local time details

	// Get the current time securely
	if (time(&now) == -1) {
		perror("Error getting current time");  // Print an error message if getting current time fails
	}

	// Convert the current time to local time structure securely
	if (localtime_s(&local_time, &now) != 0) {
		perror("Error converting to local time");  // Print an error message if conversion fails
	}

	// Print the current date and time in the format "YYYY-MM-DD HH:MM:SS"
	fprintf(ErrorFile, "The current date and time is: %04d-%02d-%02d %02d:%02d:%02d\n",
		local_time.tm_year + 1900,  // Year since 1900
		local_time.tm_mon + 1,       // Month [0, 11]
		local_time.tm_mday,          // Day of the month [1, 31]
		local_time.tm_hour,          // Hours [0, 23]
		local_time.tm_min,           // Minutes [0, 59]
		local_time.tm_sec);          // Seconds [0, 59]
}

void FetchFromDisk(Range_t range)
{
	//fetch from disk
}