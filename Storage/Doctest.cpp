#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include"doctest.h"
extern "C" {
#include"MasterCacheImages.h"
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
