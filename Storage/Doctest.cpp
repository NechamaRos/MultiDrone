#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include"doctest.h"
extern "C" {
#include"MasterCacheImages.h"
MasterCacheImg_cb_t* masterCacheImg_cb;
 
}

TEST_CASE("create point")
{
	//act
	Point_t p= createPoint(4, 5);
	//assert
	CHECK(p.x == 4);
	CHECK(p.y == 5);
}
TEST_CASE("create imgInfo")
{
	// Arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	//act
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	//assert
	
	CHECK(imgInfo->BR.x == br.x);
	CHECK(imgInfo->BR.y == br.y);
	CHECK(imgInfo->TL.x == tl.x);
	CHECK(imgInfo->TL.y == tl.y);
	CHECK(imgInfo->slaveId == 2);
	CHECK(imgInfo->imgId == 0);
	
}
TEST_CASE("Testing connect Between Both Datas")
{
	//arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	//act
	UnitNode_LRU_t* node = createUnitNode_LRU(imgInfo);
	//assert
	CHECK(imgInfo->unitNodePtr == node);
	CHECK(node->imgInfoPtr == imgInfo);
	
	
}
TEST_CASE("testing create UnitNode_LRU")
{
	// Arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	//act
	UnitNode_LRU_t* node = createUnitNode_LRU(imgInfo);
	//assert
	CHECK(node->next == NULL);
	CHECK(node->prev == NULL);
}
TEST_CASE(" testing initLinkedList")
{
	//Act
	LinkedList_LRU_t* linkedList = initLinkedList();
	//assert
	CHECK(linkedList->AmountOfLinks == 0);
	CHECK(linkedList->head->next == NULL);
	CHECK(linkedList->tail->prev==NULL);
}
TEST_CASE("insert to linked List when there is no links in the linkedList")
{
	//arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	UnitNode_LRU_t* node = createUnitNode_LRU(imgInfo);
   //act
	insertInToLinedList(node);
	//assert
	CHECK(masterCacheImg_cb->LRU->head->next == node);
	CHECK(masterCacheImg_cb->LRU->tail->prev == node);
	CHECK(masterCacheImg_cb->LRU->head == node->prev);
	CHECK(masterCacheImg_cb->LRU->tail == node->next);
	CHECK(masterCacheImg_cb->LRU->AmountOfLinks == 1);
}
TEST_CASE("insert to linked List when there is one  or more links in the linkedList")
{
	//arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	UnitNode_LRU_t* node = createUnitNode_LRU(imgInfo);
	ImgInfo_t* imgInfoA = createImgInfo(8, 2, tl, br);
	UnitNode_LRU_t* nodeA = createUnitNode_LRU(imgInfo);
	insertInToLinedList(nodeA);
	//act
	insertInToLinedList(node);
	//assert
	CHECK(masterCacheImg_cb->LRU->head->next == node);
	CHECK(masterCacheImg_cb->LRU->head == node->prev);
	CHECK(nodeA == node->next);
	CHECK(nodeA->prev == node);
	CHECK(masterCacheImg_cb->LRU->AmountOfLinks == 2);
}
TEST_CASE("move to the Beginning")
{
	//arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	UnitNode_LRU_t* node = createUnitNode_LRU(imgInfo);
	ImgInfo_t* imgInfoA = createImgInfo(8, 2, tl, br);
	UnitNode_LRU_t* nodeA = createUnitNode_LRU(imgInfo);
	insertInToLinedList(nodeA);
	insertInToLinedList(node);
	//act
	moveToTheBeginning(nodeA);
	//assert
	CHECK(masterCacheImg_cb->LRU->head->next == nodeA);
	CHECK(masterCacheImg_cb->LRU->head == nodeA->prev);
	CHECK(nodeA->next == node);
	CHECK(node->prev == nodeA);
 
}
TEST_CASE("remove when cach is full")
{
	//arrange
	Point_t tl;
	Point_t br;
	ImgInfo_t* imgInfo;
	UnitNode_LRU_t* node;
	initMasterCacheImg_cb();
	for (int i = 1;i <= 100;i++)
	{
		tl= createPoint(i, 5);
		br= createPoint(i+2, 12);
		imgInfo= createImgInfo(i, 2, tl, br);
		node = createUnitNode_LRU(imgInfo);
		insertInToLinedList(node);
	}
	//act
	removefromLinkedList();
	//assert
	CHECK(masterCacheImg_cb->LRU->AmountOfLinks ==CACHE_SIZE- (CACHE_SIZE /10));
}
TEST_CASE("Pop first empty place in cache stack ")
{
	//arrange
	initMasterCacheImg_cb();
	//act
	int index = PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	//assert
	CHECK(index == 0);
}
TEST_CASE("Pop first empty place in cache stack after a few pops")
{
	//arrange
	initMasterCacheImg_cb();
	PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	//act
	int index = PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	//assert
	CHECK(index == 3);
}
TEST_CASE("Push empty place in to stack")
{
	//arrange
	initMasterCacheImg_cb();
	int index = PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	//act
	PushEmptyPlaceInToStack(masterCacheImg_cb->emptyPlaceInCache, index);
	//assert
	CHECK(masterCacheImg_cb->emptyPlaceInCache->emptyPlaceInTheArray[CACHE_SIZE-1] == 0);
}

TEST_CASE("Push empty place in to stack after a few Pop")
{
	//arrange
	initMasterCacheImg_cb();
	PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	int index = PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	//act
	PushEmptyPlaceInToStack(masterCacheImg_cb->emptyPlaceInCache, index);
	//assert
	CHECK(masterCacheImg_cb->emptyPlaceInCache->emptyPlaceInTheArray[masterCacheImg_cb->emptyPlaceInCache->length-1] == 2);
}


