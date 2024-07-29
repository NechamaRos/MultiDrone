#pragma once
#include <stdbool.h>
#include <stdlib.h>  // For malloc, free
#define COLLECTION_SIZE 4
#define RANGE_LENGTH 1
#define RANGE_WIDTH 1
#define INIT_COUNTER_LEN 2
#define SIZE_OF_ENOUGH_SEQ 3
#define INIT_POINT { -1,-1 }
#define COUNT_OF_RANGE_TO_READING_AHEAD 2
#define INIT_RANGE {INIT_POINT,INIT_POINT}

//struct that representing a point in a plane
typedef struct Point_s
{
	int x;
	int y;
}Point_t;

//struct that representing range of two points in a plane, bottom-right and top-left
typedef struct Range_s
{
	Point_t bottomRight;
	Point_t topLeft;
}Range_t;

//enum of the optional direction of sequences
typedef enum Range_Direction_e {
	RIGHT,
	LEFT,
	UP,
	DOWN,
	IN_VALID_DIR
}Range_Direction_t;

//enum of optional types of action in the insert function
typedef enum TYPE_OF_ACTION_THAT_ACCURED_IN_INSERT_e {
	CONTINUE_SEQ,
	CREATE_SEQ,
	INSERT_TO_SINGLE_RANGES_ARRAY
}TYPE_OF_ACTION_THAT_ACCURED_IN_INSERT_t;

//struct that representing information of sequence
typedef struct SeqRangeInfo_s
{
	Range_t lastInsertedRange;
	Range_t nextExpectedRange;
	Range_Direction_t dir;
	int counterUse;
	int counterLength;
}SeqRangeInfo_t;

//control block of the program
typedef struct SequenceCollectionCtrlBlk_s {
	SeqRangeInfo_t* seqRangeInfoArray[COLLECTION_SIZE];
	Range_t singleRangesArray[COLLECTION_SIZE];
	Range_t RangesInLoadingArray[COLLECTION_SIZE];
	int singleRangeCurrentIndex;
}SequenceCollectionCtrlBlk_t;

#pragma region Range Functions

//function that init the range
Range_t GetDefaultRange();

//function that get direction, range and num, and return the next range by the direction, double the num
Range_t GetNextRangeByDirection(Range_t range, Range_Direction_t dir, int cntRangesToRead);

bool IsEqualRanges(Range_t me, Range_t other);

//function that check if currentSmallRange contained in containRange
bool IsRangeContainedInOtherRange(Range_t currentSmallRange, Range_t containRange);

//function that check if new range complete the last inserted range to sequence by some direction, if true it return the direction
Range_Direction_t IsRangeCompleteSingleRangeToSequence(Range_t range, Range_t singleRange);

#pragma endregion

#pragma region SequenceCollectionCtrlBlk Functions

//function that return the index to override by LRU
int GetLastUsedSequence();

//external function of insert new range 
void InsertNewRangeAPI(Range_t range);

//internal function that called by the external function-InsertNewRangeAPI
TYPE_OF_ACTION_THAT_ACCURED_IN_INSERT_t InsertNewRange(Range_t range);

//function that Reading ahead when the seq is enough
void ReadingAhead(SeqRangeInfo_t* seq);

//function that check if the new range continue some exist seq, and return the index of the found seq
int IsRangeContinueSequence(Range_t range);

//function that check if the new range create new seq with single range from singleRangesArray
int IsRangeCreateNewSequence(Range_t range);

//function that get index of seq that update now and up the counter use of the others seqs
void UpCounterUseOfSeqsOthers(int index);
#pragma endregion

#pragma region RangesInLoadingArray Functions

//function that return the index of the empty place of loadingArray
int GetNextEmptyIndexOfRangesInLoadingArray();



//function that find the index of range in loadingArray
int FindRangeInLoadingRangesArray(Range_t rangeForRemove);

//function that get range and add it to the loadingArray
void AddRangeToLoadingRangeArray(Range_t rangeForAdd);

bool CheckValidationOfRange(Range_t range);
#pragma endregion

void INIT();

void SendLoadingRangeToAPI(Range_t range);

void PrintCurrentTimeToErrorFile();

//void Test();
