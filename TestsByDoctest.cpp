#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "./doctest/doctest/doctest.h"

extern "C" {
#include "CachePrefetchManagement.h"
#include "CachePrefetchManagementApi.h"

	Range_t TestControlBlock[25];//control block to Test.c
	extern SequenceCollectionCtrlBlk_t* prefetchSeqCollection_CB;//extern the control block of CachePrefetchManagement.h

#pragma region PrintFunctions

	void PrintPoint(Point_t p) {
		printf(" (%d,%d) ,", p.x, p.y);
	}

	void PrintRange(Range_t range)
	{
		printf("topLeft: ");
		PrintPoint(range.topLeft);
		printf("bottomRight: ");
		PrintPoint(range.bottomRight);
		printf("\n");
	}

	void PrintSeq(SeqRangeInfo_t seq) {
		printf("lastInsertedRange: ");
		PrintRange(seq.lastInsertedRange);
		printf("nextExpectedRange: ");
		PrintRange(seq.nextExpectedRange);
		printf("counterUse: ");
		printf("%d", seq.counterUse);
		printf("\ncounter: ");
		printf("%d", seq.counter);
		printf("\nDirection: ");
		switch (seq.dir)
		{
		case 0:
			printf("RIGHT\n");
			break;
		case 1:
			printf("LEFT\n");
			break;
		case 2:
			printf("UP\n");
			break;
		case 3:
			printf("DOWN\n");
			break;
		case 4:
			printf("IN_VALID_DIR\n");
			break;
		}
		printf("\n");

	}

	void PrintSeqCollection() {

		printf("information of seq collection now:\n");

		printf("information of seqRangeInfoArray:\n");

		for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++)
		{
			printf("seqRangeInfoArray[%d]:\n", i);
			PrintSeq(*(prefetchSeqCollection_CB->seqRangeInfoArray[i]));
		}

		printf("information of singleRangesArray.array:\n");

		for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++)
		{
			printf("singleRangesArray.array[%d]:\n", i);
			PrintRange(prefetchSeqCollection_CB->singleRangesArray.array[i]);
		}

		printf("\ninformation of RangesInLoadingArray:\n");

		for (int i = 0; i < MAX_SUPPORTED_PARALLEL_RANGE; i++)
		{
			printf("RangesInLoadingArray[%d]:\n", i);
			PrintRange(prefetchSeqCollection_CB->RangesInLoadingArray[i]);
		}
	}
#pragma endregion

	void InitTestControlBlock() {
		int topLeftX;
		int topLeftY;
		int bottomRightX;
		int bottomRightY;
		Range_Direction_t dir;
		TestControlBlock[0].bottomRight = { 5, 0 };
		TestControlBlock[0].topLeft = { 1, 10 };
		TestControlBlock[1].bottomRight = { 4, 3 };
		TestControlBlock[1].topLeft = { 2, 9 };
	
		TestControlBlock[2].bottomRight = { 0, 0 };
		TestControlBlock[2].topLeft = { 0, 0 };
		TestControlBlock[3].bottomRight = {3,1 };
		TestControlBlock[3].topLeft = { 2,2 };
		TestControlBlock[4].bottomRight = { 3,2 };
		TestControlBlock[4].topLeft = { 2,3 };
		TestControlBlock[5].bottomRight = { 4,1 };
		TestControlBlock[5].topLeft = { 3, 2 };
		TestControlBlock[6].bottomRight = { 0,0 };
		TestControlBlock[6].topLeft = { 3,3 };

		TestControlBlock[7] = GetNextRangeByDirection(GetNextRangeByDirection(TestControlBlock[4], UP), UP);
		TestControlBlock[8] = GetNextRangeByDirection(GetNextRangeByDirection(TestControlBlock[3], LEFT), LEFT);
		TestControlBlock[9] = GetNextRangeByDirection(TestControlBlock[7], UP);

		topLeftX = rand() % 100 + 1;
		topLeftY = rand() % 100 + 1;
		bottomRightX = topLeftX + 1;
		bottomRightY = topLeftY - 1;
		dir =Range_Direction_t( rand() % 4);
		TestControlBlock[10].bottomRight = { bottomRightX,bottomRightY };
		TestControlBlock[10].topLeft = { topLeftX,topLeftY };
		TestControlBlock[11] = GetNextRangeByDirection(TestControlBlock[10], dir);
		TestControlBlock[12] = GetNextRangeByDirection(TestControlBlock[11], dir);
		TestControlBlock[13] = GetNextRangeByDirection(TestControlBlock[12], dir);

		topLeftX = rand() % 100 + 1;
		topLeftY = rand() % 100 + 1;
		bottomRightX = topLeftX + 1;
		bottomRightY = topLeftY - 1;
		dir =Range_Direction_t( rand() % 4);

		TestControlBlock[14].bottomRight = { bottomRightX,bottomRightY };
		TestControlBlock[14].topLeft = { topLeftX,topLeftY };
		TestControlBlock[15] = GetNextRangeByDirection(TestControlBlock[14], dir);
		TestControlBlock[16] = GetNextRangeByDirection(TestControlBlock[15], dir);
		TestControlBlock[17] = GetNextRangeByDirection(TestControlBlock[16], dir);

		topLeftX = rand() % 100 + 1;
		topLeftY = rand() % 100 + 1;
		bottomRightX = topLeftX + 1;
		bottomRightY = topLeftY - 1;
		dir =Range_Direction_t( rand() % 4);

		TestControlBlock[18].bottomRight = { bottomRightX,bottomRightY };
		TestControlBlock[18].topLeft = { topLeftX,topLeftY };
		TestControlBlock[19] = GetNextRangeByDirection(TestControlBlock[18], dir);
		TestControlBlock[20] = GetNextRangeByDirection(TestControlBlock[19], dir);
		TestControlBlock[21] = GetNextRangeByDirection(TestControlBlock[20], dir);

		topLeftX = rand() % 100 + 1;
		topLeftY = rand() % 100 + 1;
		bottomRightX = topLeftX + 1;
		bottomRightY = topLeftY - 1;
		dir =Range_Direction_t( rand() % 4);

		TestControlBlock[22].bottomRight = { bottomRightX,bottomRightY };
		TestControlBlock[22].topLeft = { topLeftX,topLeftY };
		TestControlBlock[23] = GetNextRangeByDirection(TestControlBlock[22], dir);
		TestControlBlock[24] = GetNextRangeByDirection(TestControlBlock[23], dir);
		TestControlBlock[25] = GetNextRangeByDirection(TestControlBlock[24], dir);
	}

	void TestIsRangeContainedInOtherRange() {
		CHECK(IsRangeContainedInOtherRange(TestControlBlock[1], TestControlBlock[0]));
		CHECK(IsRangeContainedInOtherRange(TestControlBlock[1], TestControlBlock[1]));
		CHECK(IsRangeContainedInOtherRange(TestControlBlock[0], TestControlBlock[0]));
		CHECK_FALSE(IsRangeContainedInOtherRange(TestControlBlock[0], TestControlBlock[1]));
	}

	void TestIsRangeCompleteSingleRangeToSequence() {
		CHECK(IsRangeCompleteSingleRangeToSequence(TestControlBlock[4], TestControlBlock[3]) == UP);
		CHECK(IsRangeCompleteSingleRangeToSequence(TestControlBlock[3], TestControlBlock[4]) == DOWN);
		CHECK(IsRangeCompleteSingleRangeToSequence(TestControlBlock[3], TestControlBlock[3]) == IN_VALID_DIR);
		CHECK(IsRangeCompleteSingleRangeToSequence(TestControlBlock[5], TestControlBlock[3]) == RIGHT);
		CHECK(IsRangeCompleteSingleRangeToSequence(TestControlBlock[3], TestControlBlock[5]) == LEFT);
	}

	void TestGetNextRangeByDirection() {

		/*PrintRange(TestControlBlock[4]);

		PrintRange(GetNextRangeByDirection(TestControlBlock[3], UP, 1));*/

		CHECK(IsEqualRanges(GetNextRangeByDirection(TestControlBlock[3], UP), TestControlBlock[4]));
		CHECK(IsEqualRanges(GetNextRangeByDirection(TestControlBlock[4], DOWN), TestControlBlock[3]));
		CHECK(IsEqualRanges(GetNextRangeByDirection(TestControlBlock[5], LEFT), TestControlBlock[3]) );
		CHECK(IsEqualRanges(GetNextRangeByDirection(TestControlBlock[3], RIGHT), TestControlBlock[5]));
		CHECK(IsRangeCompleteSingleRangeToSequence(GetNextRangeByDirection(TestControlBlock[3], UP), TestControlBlock[3]) == UP  );
		CHECK(IsRangeCompleteSingleRangeToSequence(GetNextRangeByDirection(TestControlBlock[3], DOWN), TestControlBlock[3]) == DOWN);
		CHECK(IsRangeCompleteSingleRangeToSequence(GetNextRangeByDirection(TestControlBlock[3], RIGHT), TestControlBlock[3]) == RIGHT);
		CHECK(IsRangeCompleteSingleRangeToSequence(GetNextRangeByDirection(TestControlBlock[3], LEFT), TestControlBlock[3]) == LEFT );

	}

	void TestIsEqualRanges()
	{
		CHECK_FALSE(IsEqualRanges(TestControlBlock[1], TestControlBlock[0]) );
		CHECK(IsEqualRanges(TestControlBlock[0], TestControlBlock[0]) );

	}

	void TestPrefetch_RemoveRangeFromLoadingRanges() {

		CHECK(GetNextEmptyIndexOfRangesInLoadingArray() == 3);

		Prefetch_RemoveRangeFromLoadingRanges(TestControlBlock[8]);
		CHECK_FALSE(Prefetch_IsRangeInLoading(TestControlBlock[8]));
		CHECK(FindRangeInLoadingRangesArray(TestControlBlock[8]) ==-1);
		CHECK(GetNextEmptyIndexOfRangesInLoadingArray() == 2);

		Prefetch_RemoveRangeFromLoadingRanges(TestControlBlock[7]);
		CHECK_FALSE(Prefetch_IsRangeInLoading(TestControlBlock[7]));
		CHECK(FindRangeInLoadingRangesArray(TestControlBlock[7]) == -1);
		CHECK(GetNextEmptyIndexOfRangesInLoadingArray() == 0);

		Prefetch_RemoveRangeFromLoadingRanges(TestControlBlock[9]);
		CHECK_FALSE(Prefetch_IsRangeInLoading(TestControlBlock[9]));
		CHECK(FindRangeInLoadingRangesArray(TestControlBlock[9]) == -1);
		CHECK(GetNextEmptyIndexOfRangesInLoadingArray() == 0);

	}

	void TestInsertNewRange() {

#pragma region 	the first insert has to insert to the singleRangeArray[0]

		CHECK(InsertNewRange(TestControlBlock[3]) == INSERT_TO_SINGLE_RANGES_ARRAY );
		CHECK(IsEqualRanges(prefetchSeqCollection_CB->singleRangesArray.array[0], TestControlBlock[3]));
		printf("CHECK(InsertNewRange(TestControlBlock[3])== INSERT_TO_SINGLE_RANGES_ARRAY\n\n");

		PrintSeqCollection();
#pragma endregion


#pragma region the second insert
		//the second insert 
		//has to insert to the seqRangeInfoArray[0] new sequence 
		// with the current and prev ranges and delete the prev insert from the singleRangeArray[0]

		CHECK(InsertNewRange(TestControlBlock[4]) == CREATE_SEQ );
		CHECK(IsEqualRanges(prefetchSeqCollection_CB->singleRangesArray.array[3], GetDefaultRange()));
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[0]->counter == 2);
		printf("CHECK(InsertNewRange(TestControlBlock[4]) == CREATE_SEQ\n\n");

		PrintSeqCollection();

#pragma endregion


#pragma region the third insert has to update the seqRangeinfoArray[0]

		CHECK(InsertNewRange(GetNextRangeByDirection(TestControlBlock[4], UP)) == CONTINUE_SEQ );
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[0]->counter == 3);
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[0]->counterUse == 0);
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[1]->counter == -1);
		CHECK(Prefetch_IsRangeInLoading(TestControlBlock[7]));
		CHECK(FindRangeInLoadingRangesArray(TestControlBlock[7]) != -1);

		printf("InsertNewRange(TestControlBlock[7]) == CONTINUE_SEQ\n");

		PrintSeqCollection();
#pragma endregion


#pragma region 	the fourth insert has to insert to the singleRangeArray[0]

		CHECK(InsertNewRange(TestControlBlock[5]) == INSERT_TO_SINGLE_RANGES_ARRAY );
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[0]->counter == 3);
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[0]->counterUse == 0);//stay 0 because seqRangeInfoArray wasn't changed
		CHECK(IsEqualRanges(prefetchSeqCollection_CB->singleRangesArray.array[1], TestControlBlock[5]));//singleRangeCurrentIndex was up

		printf("InsertNewRange(TestControlBlock[4]) == INSERT_TO_SINGLE_RANGES_ARRAY\n\n");

		PrintSeqCollection();

#pragma endregion


#pragma region 	the fiveth insert has to update  the seqRangeinfoArray[0]


		CHECK(InsertNewRange(GetNextRangeByDirection(GetNextRangeByDirection(TestControlBlock[4], UP), UP)) == CONTINUE_SEQ   );
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[0]->counter == 4);
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[0]->counterUse == 0);
		CHECK(IsEqualRanges(prefetchSeqCollection_CB->singleRangesArray.array[0], GetDefaultRange()));
		CHECK(Prefetch_IsRangeInLoading(TestControlBlock[9]));
		CHECK(FindRangeInLoadingRangesArray(TestControlBlock[9]) != -1);
		printf("InsertNewRange(GetNextRangeByDirection(GetNextRangeByDirection(TestControlBlock[3], UP, 1), UP, 1)) == CONTINUE_SEQ\n\n");

		PrintSeqCollection();
#pragma endregion


#pragma region the sixth insert
		// has to insert to the seqRangeInfoArray[1] 
		// new sequence with the current and prev ranges (from singleRangeArray[1])
		// and delete the prev insert from the singleRangeArray[1]

		CHECK(InsertNewRange(TestControlBlock[3]) == CREATE_SEQ);
		PrintSeqCollection();
		CHECK(IsEqualRanges(prefetchSeqCollection_CB->singleRangesArray.array[1], GetDefaultRange()));
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[1]->counter == 2);
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[1]->counterUse == 0);
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[0]->counterUse > 0);
		printf("InsertNewRange(TestControlBlock[0]) == CREATE_SEQ\n\n");
		PrintSeqCollection();
#pragma endregion


#pragma region the seventh insert has to update the seqRangeinfoArray[1]
		CHECK(InsertNewRange(GetNextRangeByDirection(TestControlBlock[3], LEFT)) == CONTINUE_SEQ );
		//change the next expected range of TestControlBlock[3], to be valid value
		TestControlBlock[8].topLeft.x = 0;
		PrintSeqCollection();
		CHECK(Prefetch_IsRangeInLoading(TestControlBlock[8]));
		CHECK(FindRangeInLoadingRangesArray(TestControlBlock[8]) != -1);
		printf("InsertNewRange(GetNextRangeByDirection(TestControlBlock[3], LEFT, 1)== CONTINUE_SEQ\n\n");
		PrintSeqCollection();
#pragma endregion

	}

	void TestInsertNewRangeRandom() {

		CHECK(InsertNewRange(TestControlBlock[10]) == INSERT_TO_SINGLE_RANGES_ARRAY);
		CHECK(InsertNewRange(TestControlBlock[14]) == INSERT_TO_SINGLE_RANGES_ARRAY);

		CHECK(InsertNewRange(TestControlBlock[11]) == CREATE_SEQ);
		CHECK(InsertNewRange(TestControlBlock[15]) == CREATE_SEQ);

		CHECK(InsertNewRange(TestControlBlock[12]) == CONTINUE_SEQ);
		CHECK(InsertNewRange(TestControlBlock[16]) == CONTINUE_SEQ);

		CHECK(InsertNewRange(TestControlBlock[13]) == CONTINUE_SEQ);
		CHECK(InsertNewRange(TestControlBlock[17]) == CONTINUE_SEQ);

		CHECK(InsertNewRange(TestControlBlock[18]) == INSERT_TO_SINGLE_RANGES_ARRAY);
		CHECK(InsertNewRange(TestControlBlock[19]) == CREATE_SEQ);
		CHECK(InsertNewRange(TestControlBlock[20]) == CONTINUE_SEQ);
		CHECK(InsertNewRange(TestControlBlock[21]) == CONTINUE_SEQ);

	}
	void TestOverrideSeqByCounterUse() {
		//insert to seq in seqRangeInfoArray[1]
		CHECK(InsertNewRange(prefetchSeqCollection_CB->seqRangeInfoArray[1]->nextExpectedRange) == CONTINUE_SEQ);

		CHECK(InsertNewRange(TestControlBlock[22]) == INSERT_TO_SINGLE_RANGES_ARRAY);
		CHECK(InsertNewRange(TestControlBlock[23]) == CREATE_SEQ);
		CHECK(InsertNewRange(TestControlBlock[24]) == CONTINUE_SEQ);
		CHECK(InsertNewRange(TestControlBlock[25]) == CONTINUE_SEQ);

		//check if the next seq inserted to index 2 in seqRangeInfoArray
		CHECK(prefetchSeqCollection_CB->seqRangeInfoArray[2]->counterUse == 0);

		PrintSeqCollection();
	}

	void TestValidationInput()
	{
		Range_t range;
		range.bottomRight = TestControlBlock[0].bottomRight;
		range.topLeft = TestControlBlock[1].topLeft;

		Prefetch_InsertNewRange(GetDefaultRange());
		Prefetch_InsertNewRange(range);

	}

	TEST_CASE("testing the IsRangeContainedInOtherRange function") {
	InitTestControlBlock();
	TestIsRangeContainedInOtherRange();
	}

	TEST_CASE("testing the TestIsRangeCompleteSingleRangeToSequence function") {
		TestIsRangeCompleteSingleRangeToSequence();
	}

	TEST_CASE("testing the TestGetNextRangeByDirection function") {
		TestGetNextRangeByDirection();
	}

	TEST_CASE("testing the TestIsEqualRanges function") {
		TestIsEqualRanges();
	}

	TEST_CASE("TestInsertNewRange") {
		Prefetch_INIT();
		TestInsertNewRange();
	}
	TEST_CASE("TestInsertNewRange") {
		TestInsertNewRangeRandom();
	}
	TEST_CASE("TestOverrideSeqByCounterUse") {
		TestOverrideSeqByCounterUse();
	}
	TEST_CASE("TestCheckValidation") {
		TestValidationInput();
	}
}