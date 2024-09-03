#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cstdint>  
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

	freeMasterCacheImg_cb();
	
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
	freeMasterCacheImg_cb();
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
	freeMasterCacheImg_cb();
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
	int* a = init_imgData();
	
    //act
	insertData(node, a);
	//assert
	CHECK(masterCacheImg_cb->LRU->head->next == node);
	CHECK(masterCacheImg_cb->LRU->tail->prev == node);
	CHECK(masterCacheImg_cb->LRU->head == node->prev);
	CHECK(masterCacheImg_cb->LRU->tail == node->next);
	CHECK(masterCacheImg_cb->LRU->AmountOfLinks == 1);
	freeMasterCacheImg_cb();
}
TEST_CASE("insert to image array")
{
	//arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	//act
	insertToImgArray(imgInfo);
	//assert
	CHECK(masterCacheImg_cb->imgArray[0] == imgInfo);
	freeMasterCacheImg_cb();

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
	UnitNode_LRU_t* nodeA = createUnitNode_LRU(imgInfoA);
	int* a = init_imgData();
	int* s = init_imgData();
	insertData(nodeA,s);	
	//act
	insertData(node, a);
	//assert
	CHECK(masterCacheImg_cb->LRU->head->next == node);
	CHECK(masterCacheImg_cb->LRU->head == node->prev);
	CHECK(nodeA == node->next);
	CHECK(nodeA->prev == node);
	CHECK(masterCacheImg_cb->LRU->AmountOfLinks == 2);
	freeMasterCacheImg_cb();
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
	UnitNode_LRU_t* nodeA = createUnitNode_LRU(imgInfoA);
	int* a = init_imgData();
	int* s = init_imgData();
	insertData(nodeA, s);
	insertData(node, a);
	//act
	moveToTheBeginning(nodeA);
	//assert
	CHECK(masterCacheImg_cb->LRU->head->next == nodeA);
	CHECK(masterCacheImg_cb->LRU->head == nodeA->prev);
	CHECK(nodeA->next == node);
	CHECK(node->prev == nodeA);
	freeMasterCacheImg_cb();
}
TEST_CASE("insert data")
{
	//arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	UnitNode_LRU_t* node = createUnitNode_LRU(imgInfo);
	int* a = init_imgData();
	//act
	insertData(node,a);
	CHECK(masterCacheImg_cb->cache[0] == a);
	CHECK(masterCacheImg_cb->imgArray[0] == imgInfo);
	CHECK(imgInfo->arrayIndexPtr == &masterCacheImg_cb->imgArray[0]);
	CHECK(imgInfo->cachePtr == &masterCacheImg_cb->cache[0]);
	CHECK(masterCacheImg_cb->emptyPlaceInCache->length == 99);
	CHECK(masterCacheImg_cb->emptyPlaceInTheArray->length == 99);
	freeMasterCacheImg_cb();
}
TEST_CASE("insert two images to data")
{
	//arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	ImgInfo_t* imgInfo = createImgInfo(0, 2, tl, br);
	ImgInfo_t* imgInfoA = createImgInfo(0, 1, tl, br);
	UnitNode_LRU_t* node = createUnitNode_LRU(imgInfo);
	UnitNode_LRU_t* nodeA = createUnitNode_LRU(imgInfoA);
	int* a = init_imgData();
	insertData(nodeA, a);
	//act
	insertData(node, a);
	//assert
	CHECK(masterCacheImg_cb->cache[1] == a);
	CHECK(masterCacheImg_cb->imgArray[1] == imgInfo);
	CHECK(imgInfo->arrayIndexPtr == &masterCacheImg_cb->imgArray[1]);
	//CHECK(imgInfo->cachePtr == &masterCacheImg_cb->cache[1]);
	CHECK(masterCacheImg_cb->emptyPlaceInCache->length == 98);
	CHECK(masterCacheImg_cb->emptyPlaceInTheArray->length == 98);
	freeMasterCacheImg_cb();
}
TEST_CASE("remove when cach is full")
{
	//arrange
	Point_t tl;
	Point_t br;
	ImgInfo_t* imgInfo;
	UnitNode_LRU_t* node;
	int* imgData=NULL;
	int* a;
	initMasterCacheImg_cb();
	for (int i =0;i < 100;i++)
	{
		tl= createPoint(i, 6);
		br= createPoint(i+2, 12);
		imgInfo= createImgInfo(i, 2, tl, br);
		node = createUnitNode_LRU(imgInfo);
		a = init_imgData();
		insertData(node,a);
	}
	tl = createPoint(9, 5);
	br = createPoint(18, 5);
	imgInfo = createImgInfo(8, 1, tl, br);
	node = createUnitNode_LRU(imgInfo);
	a = init_imgData();
	//act
	insertData(node, a);
	//assert
	CHECK(masterCacheImg_cb->LRU->AmountOfLinks ==CACHE_SIZE- (CACHE_SIZE /10)+1);
	freeMasterCacheImg_cb();
}
TEST_CASE("Pop first empty place in cache stack ")
{
	//arrange
	initMasterCacheImg_cb();
	//act
	int index = PopFirstEmptyPlaceInStack(masterCacheImg_cb->emptyPlaceInCache);
	//assert
	CHECK(index == 0);
	freeMasterCacheImg_cb();
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
	freeMasterCacheImg_cb();
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
	freeMasterCacheImg_cb();
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
	freeMasterCacheImg_cb();
}

TEST_CASE("init stack all place is empty")
{
	//act
	Stack_emptyPlace_t* stack = initStack();
	//assert
	CHECK(stack->length == CACHE_SIZE);
	CHECK(stack->emptyPlaceInTheArray[0] == CACHE_SIZE - 1);
	CHECK(stack->emptyPlaceInTheArray[CACHE_SIZE - 1] == 0);
	free(stack);
}
TEST_CASE("Testing insert buffer in to cache")
{
	//arrange
	initMasterCacheImg_cb();
	Point_t tl = createPoint(7, 5);
	Point_t br = createPoint(5, 12);
	SlaveImgInfo_t* slave = createSlaveImgInfo(0, 0, tl, br);
	SlaveImgInfo_t* slaveA = createSlaveImgInfo(0, 2, tl, br);
	SlaveImgInfo_t* slaveB = createSlaveImgInfo(1, 1, tl, br);
	SlaveImgInfo_t* slaveArr[] = { slave,slaveA,slaveB };
	//Act
	insertBufferInToCache(slaveArr, 3);
	//assert
	CHECK(masterCacheImg_cb->cache[0] == slave->data);
	CHECK(masterCacheImg_cb->cache[1] == slaveA->data);
	CHECK(masterCacheImg_cb->cache[2] == slaveB->data);

	//free
	free(slave);
	free(slaveA);
	free(slaveB);
	freeMasterCacheImg_cb();


}
//TEST_CASE("insert to cache")
//{
//	//arrange
//	int* imgData = NULL;
//	int index = CACHE_SIZE - (masterCacheImg_cb->emptyPlaceInCache->length + 1);
//	//act
//	insertTocache(imgData,);
//	//assert
//	//CHECK(masterCacheImg_cb->cache[0] == imgData);
//}
 
