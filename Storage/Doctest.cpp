#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include"doctest.h"
#include <stdlib.h>
#include <random>


extern "C" {
#include"DiskManagmentApi.h"
#include"Mock.h"
	extern DiskMangmantCb_t* diskMangmantCb;
}

int randomValue(int lower,int upper)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distrib(lower, upper);

	// random number
	int randomNumber = distrib(gen);
	return randomNumber;
}

TEST_CASE("Testing creat point with positive numbers") {
	// Arrange
	int x = randomValue(0, 40);
	int y = randomValue(0, 40);
	// Act
	Point_t p = CreatePoint(x, y);

	// Assert
	CHECK(p.y == y);
	CHECK(p.x == x);
}
TEST_CASE("Testing creat point with negative numbers") {
	// Arrange
	int x = randomValue(-110, 40);;
	int y = randomValue(-100, 0);
	// Act
	Point_t p = CreatePoint(x, y);

	// Assert
	CHECK(errno == POINT_NEGATIVE_ERORR);
}
TEST_CASE("test_CreateImagePoint_ValidInput")
{
	//Arrange
	int x= randomValue(0, 38);
	int y= randomValue(0, 38);
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);

	//Act
	ImagePoints_t point = CreateImagePoint(a, b);

	//Assert
	CHECK(point.BR.x == b.x);
	CHECK(point.BR.y == b.y);
	CHECK(point.TL.y == a.y);
	CHECK(point.TL.x == a.x);
}
TEST_CASE("test_CreateImagePoint_not_in_range")
{
	//Arrange
	int x= randomValue(0, 40);
	int y= randomValue(0, 40);
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+41, y+SIZE_OF_IMG_WIDTH);

	//Act
	ImagePoints_t point = CreateImagePoint(a, b);

	//Assert
	CHECK(errno == POINT_NOT_IN_RANGE_ERORR);
}
TEST_CASE("test CreateImagePoint not CorrectPoints tl bigger")
{
	//Arrange
	int x = randomValue(2, 38);
	int y = randomValue(2, 38);

	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x - SIZE_OF_IMG_LENGTH, y + SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);

	//Assert
	CHECK(errno == POINT_NOT_IN_RANGE_ERORR);
}
TEST_CASE("TestingImgInfo ValidInput")
{
	//Arrange
	int x= randomValue( 0,  38);
	int y = randomValue(0, 38);
	//points for the img range
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* data = NULL;
	//Act
	ImageInfo_t* info = CreateImageInfo(point, data);
	//Assert
	CHECK(info->imgPoints.TL.x == point.TL.x);
	CHECK(info->imgPoints.TL.y == point.TL.y);
	CHECK(info->imgPoints.BR.x == point.BR.x);
	CHECK(info->imgPoints.BR.y == point.BR.y);
	CHECK(info->disk_ptr == NULL);
	//freeing allocated memory
	free(info);
}
TEST_CASE("Testing create LinkList")
{
	//Arrange

	//Act
	LinkedList_t* l = createLinkedList();
	//Assert
	CHECK(l->head->next == NULL);
	CHECK(l->tail->prev == NULL);
	CHECK(l->AmountOfLinks == 0);

	//freeing allocated memory
	free(l->head);
	free(l->tail);
	free(l);
}
TEST_CASE("Testing create LinkList Node")
{

	//Arrange
	int x=randomValue(0,38);
	int y=randomValue(0,38);
	//the info of the img for the node
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* imgData =NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	//Act
	UnitNodeLinkedList_t* node = createNode(info);
	//Assert
	CHECK(node->next == NULL);
	CHECK(node->prev == NULL);
	CHECK(node->next == NULL);
	CHECK(node->prev == NULL);
	CHECK(node->imgInfo == info);

	//freeing allocated memory
	free(info);
	free(node);
}
TEST_CASE("Testing create QuadNode")
{
	//Arrange
	int x = randomValue(0,38);
	int y = randomValue(0,38);
	//creating node
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* imgData =NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	UnitNodeLinkedList_t* node = createNode(info);
	//Act
	QuadNode_t* quadNode = createQuadNode(point, node);
	//assert
	CHECK(quadNode->imagePoints.TL.x == point.TL.x);
	CHECK(quadNode->imagePoints.TL.y == point.TL.y);
	CHECK(quadNode->imagePoints.BR.x == point.BR.x);
	CHECK(quadNode->imagePoints.BR.y == point.BR.y);
	CHECK(quadNode->parent == NULL);

	//freeing allocated memory
	free(info);
	free(quadNode);
	free(node);
}
TEST_CASE("Testing connector between data structures")
{
	//Arrange
	int x = randomValue(0,38);
	int y = randomValue(0,38);
	//creating a node
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* imgData = NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	UnitNodeLinkedList_t* node = createNode(info);
	//Act
	QuadNode_t* quadNode = createQuadNode(point, node);
	//assert
	CHECK(quadNode->LinkedList_ptr == node);
	CHECK(node->positionOnTree_ptr == quadNode);

	//freeing allocated memory
	free(info);
	free(quadNode);
	free(node);
}
TEST_CASE("Testing create QuadTree")
{
	int x=randomValue(0, 38);
	int y=randomValue(0, 38);
	//Arrange
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	//Act
	QuadTree_t* quadTree = createQuadTree(a, b);
	//Assert

	CHECK(quadTree->TL.x == a.x);
	CHECK(quadTree->TL.y == a.y);
	CHECK(quadTree->BR.x == b.x);
	CHECK(quadTree->BR.y == b.y);
	CHECK(quadTree->botLeftTree == NULL);
	CHECK(quadTree->botRightTree == NULL);
	CHECK(quadTree->topLeftTree == NULL);
	CHECK(quadTree->topRightTree == NULL);
	CHECK(quadTree->parent == NULL);
	CHECK(quadTree->quadNode == NULL);

	//freeing allocated memory
	free(quadTree);
	
}
TEST_CASE("Testing insert to linkList when linkList is mull")
{
	//Arrange
    int x = randomValue(0, 38);
	int y = randomValue(0, 38);
	//Creating a node
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* imgData =NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	UnitNodeLinkedList_t* node = createNode(info);
	//Creating a linked list
	initDiskMangmantCb();
	//Act
	insertToLinkList(node);

	//Assert
	CHECK(diskMangmantCb->linkedList->head != NULL);
	CHECK(diskMangmantCb->linkedList->tail != NULL);
	/// Maybe it's not good because it's member
	CHECK(diskMangmantCb->linkedList->AmountOfLinks == 1);
	CHECK(diskMangmantCb->linkedList->head->next == node);
	CHECK(diskMangmantCb->linkedList->tail->prev == node);
	CHECK(diskMangmantCb->linkedList->head->next->imgInfo == info);
	CHECK(node->next == diskMangmantCb->linkedList->tail);
	CHECK(node->prev == diskMangmantCb->linkedList->head);

	//freeing allocated memory
	free(info);
	free(node);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);

}
TEST_CASE("insert to linkList when It is  not null")
{
	//Arrange
	int x = randomValue(0, 36);
	int y = randomValue(0, 36);
	//creating points
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	Point_t c = CreatePoint(x+SIZE_OF_IMG_LENGTH*2, y+SIZE_OF_IMG_WIDTH*2);
	//img info
	ImagePoints_t point_ = CreateImagePoint(a, b);
	ImagePoints_t point = CreateImagePoint(b, c);
	int* imgData =NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	ImageInfo_t* imgInfo = CreateImageInfo(point_, imgData);
	//creating nodes to insert
	UnitNodeLinkedList_t* node = createNode(info);
	UnitNodeLinkedList_t* secondNode = createNode(imgInfo);
	//Creating a link list
	initDiskMangmantCb();
	insertToLinkList(node);
	//Act
	insertToLinkList(secondNode);
	//Assert
	CHECK(diskMangmantCb->linkedList->head->next == secondNode);
	CHECK(node->prev == secondNode);
	CHECK(secondNode->imgInfo == imgInfo);
	CHECK(secondNode->next == node);
	CHECK(secondNode->prev == diskMangmantCb->linkedList->head);

	//freeing allocated memory;
	free(info);
	free(node);
	free(imgInfo);
	free(secondNode);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
}
TEST_CASE("Testing remove node from linkedList when there is only onel ink")
{
	//Arrange			
	int x = randomValue(0, 38);
	int y = randomValue(0, 38);
	//creating points
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, x+SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* imgData = NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	//creating liinedList
	initDiskMangmantCb();
	insertToLinkList(node);
	//Act
	removeNodeFromLinkedList();
	//Assert

	CHECK(diskMangmantCb->linkedList->head->next == NULL);
	CHECK(diskMangmantCb->linkedList->tail->prev == NULL);

	//freeing allocated memory
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("Testing remove node from linkedList when there is only one link")
{
	//Arrange
	int x=randomValue(0, 38);
	int y=randomValue(0, 38);
	//creating points
	Point_t a = CreatePoint(6, 16);
	Point_t b = CreatePoint(8, 18);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* imgData = NULL;
	//img info
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	//creating liinedList
	initDiskMangmantCb();
	insertToLinkList(node);
	//Act
	removeNodeFromLinkedList();
	//Assert

	CHECK(diskMangmantCb->linkedList->head->next == NULL);
	CHECK(diskMangmantCb->linkedList->tail->prev == NULL);

	//freeing allocated memory
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("Testing remove node from linkedList")
{
	//Arrange
	int x =randomValue(0, 34);
	int y =randomValue(0, 34);
	//creating points
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	Point_t c = CreatePoint(x + SIZE_OF_IMG_LENGTH*2, y + SIZE_OF_IMG_WIDTH*2);
	Point_t d = CreatePoint(x + SIZE_OF_IMG_LENGTH*3, y + SIZE_OF_IMG_WIDTH*2);

	ImagePoints_t PointA = CreateImagePoint(b, c);
	ImagePoints_t point = CreateImagePoint(a, b);
	ImagePoints_t pointB = CreateImagePoint(c, d);
	int* imgData = NULL;
	//img info
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	ImageInfo_t* imgInfo = CreateImageInfo(point, imgData);
	ImageInfo_t* infoC = CreateImageInfo(point, imgData);

	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	UnitNodeLinkedList_t* nodeA = createNode(imgInfo);
	UnitNodeLinkedList_t* nodeB = createNode(infoC);

	//creating liinedList
	initDiskMangmantCb(); //insert to linkedList
	insertToLinkList(node);
	insertToLinkList(nodeA);
	insertToLinkList(nodeB);
	//Act
	removeNodeFromLinkedList();
	//Assert

	CHECK(diskMangmantCb->linkedList->tail == nodeA->next);
	CHECK(diskMangmantCb->linkedList->tail->prev == nodeA);

	//freeing allocated memory
	free(info);
	free(infoC);
	free(imgInfo);
	free(node);
	free(nodeB);
	free(nodeA);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
	
}
TEST_CASE("Testing remove node from linkedList when there is only one link and adding a new node")
{
	//Arrange
	int x=randomValue(0, 38);
	int y=randomValue(0, 38);
	//creating points
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y + SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* imgData = NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	//creating liinedList
	initDiskMangmantCb();
	insertToLinkList(node);
	removeNodeFromLinkedList();
	//Act
	insertToLinkList(node);
	//Assert

	CHECK(node->next == diskMangmantCb->linkedList->tail);
	CHECK(node->prev == diskMangmantCb->linkedList->head);
	CHECK(diskMangmantCb->linkedList->head->next == node);
	CHECK(diskMangmantCb->linkedList->tail->prev == node);

	//freeing allocated memory
	free(info);
	free(node);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
	
}
TEST_CASE("Testing Move to the beginning")
{
	//Arrange
	int x = randomValue(0, 34);
	int y = randomValue(0, 34);
	//creating img info for three nodes
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	Point_t c = CreatePoint(x + SIZE_OF_IMG_LENGTH*2, y + SIZE_OF_IMG_WIDTH*2);
	Point_t d = CreatePoint(x + SIZE_OF_IMG_LENGTH*3, y + SIZE_OF_IMG_WIDTH*3);
	ImagePoints_t point_ = CreateImagePoint(a, b);
	ImagePoints_t point = CreateImagePoint(b, c);
	ImagePoints_t pointC = CreateImagePoint(c, d);
	int* imgData =NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	ImageInfo_t* imgInfo = CreateImageInfo(point_, imgData);
	ImageInfo_t* imgInfoC = CreateImageInfo(pointC, imgData);
	//creating three nodes
	UnitNodeLinkedList_t* node = createNode(info);
	UnitNodeLinkedList_t* secondNode = createNode(imgInfo);
	UnitNodeLinkedList_t* thirdNode = createNode(imgInfoC);
	//creating linkedList
	initDiskMangmantCb();
	//inserting the nodes in to the list
	insertToLinkList(node);
	insertToLinkList(secondNode);
	insertToLinkList(thirdNode);
	//Act
	moveToTheBeginning(secondNode);
	//Assert

	CHECK(diskMangmantCb->linkedList->head->next == secondNode);
	CHECK(diskMangmantCb->linkedList->head == secondNode->prev);
	CHECK(secondNode->next == thirdNode);
	CHECK(secondNode == thirdNode->prev);
	CHECK(thirdNode->next == node);
	CHECK(thirdNode == node->prev);

	//freeing allocated memory
	free(info);
	free(imgInfoC);
	free(imgInfo);
	free(node);
	free(secondNode);
	free(thirdNode);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);

}
TEST_CASE("Testing move to the beginning when there's one link")
{
	//Arrange
	int x = randomValue(0, 38);
	int y = randomValue(0, 38);
	//creating img info
	Point_t a = CreatePoint(x, y);
	Point_t b = CreatePoint(x + SIZE_OF_IMG_LENGTH, y + SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(a, b);
	int* imgData = NULL;
	//img info
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	//creating linkedList
	initDiskMangmantCb();
	insertToLinkList(node);
	//Act
	moveToTheBeginning(node);
	//Assert
	CHECK(diskMangmantCb->linkedList->head == node->prev);
	CHECK(diskMangmantCb->linkedList->tail == node->next);
	CHECK(diskMangmantCb->linkedList->head->next == node);
	CHECK(diskMangmantCb->linkedList->tail->prev == node);

	//freeing allocated memory
	free(info);
	free(node);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
}

TEST_CASE("Testing insert to the Quadtree when QuadTree is empty")
{
	//Arrange
	int x=randomValue(0, 38);
	int y=randomValue(0, 38);
	//creating QUADTREE
	initDiskMangmantCb();
	//creating img info
	Point_t c = CreatePoint(x, y);
	Point_t d = CreatePoint(x + SIZE_OF_IMG_LENGTH, y + SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(c, d);
	int* imgData =NULL;
	//img info
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	//creating QuadNode
	QuadNode_t* quadNode = createQuadNode(point, node);
	//Act
	insertTotheQuadtree(quadNode, diskMangmantCb->quadTree);
	//Assert
	CHECK(diskMangmantCb->quadTree->quadNode == quadNode);
	CHECK(diskMangmantCb->quadTree->topLeftTree == NULL);
	CHECK(diskMangmantCb->quadTree->topRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->botLeftTree == NULL);
	CHECK(diskMangmantCb->quadTree->botRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->parent == NULL);

	//freeing allocated memory
	free(info);
	free(quadNode);
	free(node);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
	printf("19");
}
TEST_CASE("Testing insert to the Quadtree")
{
	//Arrange

	//creating QuadTree
	initDiskMangmantCb();
	//creating img info
	Point_t c = CreatePoint(1, 1);
	Point_t d = CreatePoint(3, 3);
	Point_t x = CreatePoint(2, 2);
	Point_t y = CreatePoint(4, 4);
	ImagePoints_t point = CreateImagePoint(c, d);
	ImagePoints_t pointB = CreateImagePoint(x, y);
	int* imgData =NULL;
	//img info
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	ImageInfo_t* imgInfo = CreateImageInfo(pointB, imgData);
	//creating nodes
	UnitNodeLinkedList_t* node = createNode(info);
	UnitNodeLinkedList_t* nodeB = createNode(imgInfo);
	//creating QuadNode
	QuadNode_t* quadNode = createQuadNode(point, node);
	QuadNode_t* quadNodeB = createQuadNode(pointB, nodeB);

	insertTotheQuadtree(quadNode, diskMangmantCb->quadTree);
	//Act
	insertTotheQuadtree(quadNodeB, diskMangmantCb->quadTree);
	//Assert
	CHECK(diskMangmantCb->quadTree->botLeftTree == NULL);
	CHECK(diskMangmantCb->quadTree->botRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->topRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->numOfNonNULLQuadTrees == 1);
	CHECK(quadNode->parent->botLeftTree == NULL);
	CHECK(quadNode->parent->topRightTree == NULL);
	CHECK(quadNode->parent->quadNode == quadNode);
	CHECK(quadNodeB->parent->quadNode == quadNodeB);
	CHECK(quadNode->parent->numOfNonNULLQuadTrees == 0);
	CHECK(quadNodeB->parent->numOfNonNULLQuadTrees == 0);
	CHECK(quadNode->parent->parent->numOfNonNULLQuadTrees == 2);

	//freeing allocated memory
	free(info);
	free(imgInfo);
	free(quadNode);
	free(quadNodeB);
	free(node);
	free(nodeB);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
	printf("20");
}
TEST_CASE("Testing remove quadNode from tree when there is one quadNode")
{
	//Arrange
	//creating QuadTree
	initDiskMangmantCb();
	//creating img info
	Point_t c = CreatePoint(20, 10);
	Point_t d = CreatePoint(22, 12);
	ImagePoints_t point = CreateImagePoint(c, d);
	int* imgData =NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	//creating quadNode
	QuadNode_t* quadNode = createQuadNode(point, node);
	insertTotheQuadtree(quadNode, diskMangmantCb->quadTree);
	//Act
	removeQuadNodeFromTree(quadNode);
	//Assert
	CHECK(diskMangmantCb->quadTree->botLeftTree == NULL);
	CHECK(diskMangmantCb->quadTree->botRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->topRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->topLeftTree == NULL);
	CHECK(diskMangmantCb->quadTree->numOfNonNULLQuadTrees == 0);

	//freeing allocated memory
	free(info);
	free(node);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
	printf("21");

}
TEST_CASE("Testing remove quadNode from tree two quadNode")
{
	//Arrange
	//creating quadTree
	initDiskMangmantCb();
	//creating img info
	Point_t c = CreatePoint(1, 1);
	Point_t d = CreatePoint(3, 3);
	Point_t x = CreatePoint(2, 2);
	Point_t y = CreatePoint(4, 4);
	ImagePoints_t point = CreateImagePoint(c, d);
	ImagePoints_t pointB = CreateImagePoint(x, y);
	int* imgData =NULL;
	ImageInfo_t* imgInfo = CreateImageInfo(pointB, imgData);
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	UnitNodeLinkedList_t* nodeB = createNode(imgInfo);
	//creating quadNode
	QuadNode_t* quadNode = createQuadNode(point, node);
	QuadNode_t* quadNodeB = createQuadNode(pointB, nodeB);
	//insert
	insertTotheQuadtree(quadNode, diskMangmantCb->quadTree);
	insertTotheQuadtree(quadNodeB, diskMangmantCb->quadTree);
	//Act
	removeQuadNodeFromTree(quadNode);
	//Assert

	CHECK(diskMangmantCb->quadTree->quadNode == quadNodeB);
	CHECK(diskMangmantCb->quadTree->botLeftTree == NULL);
	CHECK(diskMangmantCb->quadTree->botRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->topLeftTree == NULL);
	CHECK(diskMangmantCb->quadTree->topRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->numOfNonNULLQuadTrees == 0);

	//freeing allocated memory
	free(imgInfo);
	free(info);
	free(quadNodeB);
	free(node);
	free(nodeB);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
	printf("22");

}
TEST_CASE("Testing remove QuadNode from tree when three chilern")
{
	//Arrange
	//creating quadTree
	initDiskMangmantCb();
	//creating img info
	Point_t c = CreatePoint(1, 1);
	Point_t d = CreatePoint(3, 3);
	Point_t x = CreatePoint(21, 2);
	Point_t y = CreatePoint(23, 4);
	Point_t s = CreatePoint(1, 21);
	Point_t z = CreatePoint(3, 23);
	ImagePoints_t point = CreateImagePoint(c, d);
	ImagePoints_t pointB = CreateImagePoint(x, y);
	ImagePoints_t pointC = CreateImagePoint(s, z);
	int* imgData =NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	ImageInfo_t* imgInfo = CreateImageInfo(pointB, imgData);
	ImageInfo_t* imgInfoC = CreateImageInfo(pointC, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	UnitNodeLinkedList_t* nodeC = createNode(imgInfoC);
	UnitNodeLinkedList_t* nodeB = createNode(imgInfo);
	//creating quadNode
	QuadNode_t* quadNodeB = createQuadNode(pointB, nodeB);
	QuadNode_t* quadNode = createQuadNode(point, node);
	QuadNode_t* quadNodeC = createQuadNode(pointC, nodeC);
	//inserting to tree
	insertTotheQuadtree(quadNode, diskMangmantCb->quadTree);
	insertTotheQuadtree(quadNodeB, diskMangmantCb->quadTree);
	insertTotheQuadtree(quadNodeC, diskMangmantCb->quadTree);
	//Act
	removeQuadNodeFromTree(quadNodeC);
	//Assert
	CHECK(diskMangmantCb->quadTree->botLeftTree == NULL);
	CHECK(diskMangmantCb->quadTree->topRightTree != NULL);
	CHECK(diskMangmantCb->quadTree->topLeftTree != NULL);
	CHECK(diskMangmantCb->quadTree->botRightTree == NULL);

	//freeing allocated memory
	free(info);
	free(imgInfoC);
	free(imgInfo);
	free(node);
	free(nodeB);
	free(nodeC);
	free(quadNode);
	free(quadNodeB);
	free(diskMangmantCb->quadTree);
	free(diskMangmantCb->linkedList->head);
	free(diskMangmantCb->linkedList->tail);
	free(diskMangmantCb->linkedList);
	free(diskMangmantCb);
}
TEST_CASE("Testing remove QuadNode from QuadTree when grandfather numOfNonNULLQuadTrees is two")
{
	//Arrange
	//creating QuadTree
	initDiskMangmantCb();
	//creating img info
	Point_t c = CreatePoint(1, 1);
	Point_t d = CreatePoint(3, 3);
	ImagePoints_t point = CreateImagePoint(c, d);
	Point_t x = CreatePoint(2, 2);
	Point_t y = CreatePoint(4, 4);
	ImagePoints_t pointB = CreateImagePoint(x, y);
	Point_t s = CreatePoint(1, 21);
	Point_t z = CreatePoint(3, 23);
	ImagePoints_t pointC = CreateImagePoint(s, z);
	int* imgData =NULL;
	ImageInfo_t* info = CreateImageInfo(point, imgData);
	ImageInfo_t* imgInfo = CreateImageInfo(pointB, imgData);
	ImageInfo_t* imgInfoC = CreateImageInfo(pointC, imgData);
	//creating node
	UnitNodeLinkedList_t* node = createNode(info);
	UnitNodeLinkedList_t* nodeB = createNode(imgInfo);
	UnitNodeLinkedList_t* nodeC = createNode(imgInfoC);
	//creating QuadNode
	QuadNode_t* quadNode = createQuadNode(point, node);
	QuadNode_t* quadNodeB = createQuadNode(pointB, nodeB);
	QuadNode_t* quadNodeC = createQuadNode(pointC, nodeC);
	//insert to tree
	insertTotheQuadtree(quadNode, diskMangmantCb->quadTree);
	insertTotheQuadtree(quadNodeB, diskMangmantCb->quadTree);
	insertTotheQuadtree(quadNodeC, diskMangmantCb->quadTree);
	//Act
	removeQuadNodeFromTree(quadNode);
	//Assert
	CHECK(diskMangmantCb->quadTree->topLeftTree->quadNode == quadNodeB);
	CHECK(diskMangmantCb->quadTree->botLeftTree != NULL);
	CHECK(diskMangmantCb->quadTree->botRightTree == NULL);
	CHECK(diskMangmantCb->quadTree->topRightTree == NULL);
	CHECK(quadNodeB->parent->botLeftTree == NULL);
	CHECK(quadNodeB->parent->botRightTree == NULL);
	CHECK(quadNodeB->parent->topLeftTree == NULL);
	CHECK(quadNodeB->parent->topRightTree == NULL);

	//freeing allocated memory
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("Testing search in QuadTree non in range")
{

	//Arrange
	initDiskMangmantCb();
	//points for data
	Point_t c = CreatePoint(1, 1);
	Point_t d = CreatePoint(3, 3);
	Point_t x = CreatePoint(2, 2);
	Point_t y = CreatePoint(4, 4);
	Point_t s = CreatePoint(1, 21);
	Point_t z = CreatePoint(3, 23);
	int* imgData =NULL;
	initImg(c, d, imgData);
	initImg(x, y, imgData);
	initImg(s, z, imgData);
	//points for range
	Point_t TL = CreatePoint(2, 2);
	Point_t BR = CreatePoint(3, 3);
	int arr[100];
	int a = 0;
	//Act
	searchImgsAtQuadTreeByRange(diskMangmantCb->quadTree, TL, BR, &a, arr);
	//Assert
	CHECK(diskMangmantCb->arraySearchInfo[0] == NULL);
	
}
TEST_CASE("Testing search in QuadTree all in range")
{

	//Arrange
	initDiskMangmantCb();
	//points for data
	Point_t c = CreatePoint(1, 1);
	Point_t d = CreatePoint(3, 3);
	Point_t x = CreatePoint(2, 2);
	Point_t y = CreatePoint(4, 4);
	Point_t s = CreatePoint(1, 21);
	Point_t z = CreatePoint(3, 23);
	int* imgData = NULL;
	initImg(c, d, imgData);
	initImg(x, y, imgData);
	initImg(s, z, imgData);
	//points for range
	Point_t TL = CreatePoint(0, 0);
	Point_t BR = CreatePoint(40, 40);
	int arr[100] = { 0 };
	int a = 0;
	//Act
	searchImgsAtQuadTreeByRange(diskMangmantCb->quadTree, TL, BR, &a, arr);
	//Assert
	CHECK(diskMangmantCb->arraySearchInfo[3] == NULL);
	CHECK(diskMangmantCb->arraySearchInfo[0] != NULL);
	CHECK(diskMangmantCb->arraySearchInfo[2] != NULL);
	CHECK(diskMangmantCb->arraySearchInfo[1] != NULL);
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("Testing search in QuadTree one is in range")
{

	//Arrange
	initDiskMangmantCb();
	//points for data
	Point_t c = CreatePoint(1, 1);
	Point_t d = CreatePoint(3, 3);
	Point_t x = CreatePoint(2, 9);
	Point_t y = CreatePoint(4, 11);
	Point_t s = CreatePoint(1, 21);
	Point_t z = CreatePoint(3, 23);
	int* imgData =NULL;
	initImg(c, d, imgData);
	initImg(x, y, imgData);
	initImg(s, z, imgData);
	//points for range
	Point_t TL = CreatePoint(0, 0);
	Point_t BR = CreatePoint(4, 5);
	int arr[100] = { 0 };
	int a = 0;
	//Act
	searchImgsAtQuadTreeByRange(diskMangmantCb->quadTree, TL, BR, &a, arr);
	//Assert
	CHECK(diskMangmantCb->arraySearchInfo[1] == NULL);
	CHECK(diskMangmantCb->arraySearchInfo[0] != NULL);
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("save Before Shutdown ")
{
	int e=randomValue(0, 38);
	int f=randomValue(0, 38);
	int a=randomValue(0, 38);
	int b=randomValue(0, 38);
	int n=randomValue(0, 38);
	int m=randomValue(0, 38);
	//arrange
	initDiskMangmantCb();
	Point_t c = CreatePoint(e, f);
	Point_t d = CreatePoint(e + SIZE_OF_IMG_LENGTH, f + SIZE_OF_IMG_WIDTH);
	Point_t x = CreatePoint(a, b);
	Point_t y = CreatePoint(a + SIZE_OF_IMG_LENGTH, b + SIZE_OF_IMG_WIDTH);
	Point_t s = CreatePoint(n, m);
	Point_t z = CreatePoint(n + SIZE_OF_IMG_LENGTH, m + SIZE_OF_IMG_WIDTH);
	int* imgData =NULL;
	initImg(c, d, imgData);
	initImg(x, y, imgData);
	initImg(s, z, imgData);
	//act
	saveBeforeShutdown();
	//assert
	
	
}
TEST_CASE("boot When The Device Is TurnedOn")
{
	//act
	bootWhenTheDeviceIsTurnedOn();
}
TEST_CASE("Cache is asking to load a deleted image ")
{
	//arrange
	int x= randomValue(1, 38);
	int y= randomValue(1, 38);
	int a= randomValue(1, 38);
	int b= randomValue(1, 38);
	int x1=randomValue(1, 38);
	int y1=randomValue(1, 38);
	initDiskMangmantCb();
	//points for data
	Point_t c = CreatePoint(x, y);
	Point_t d = CreatePoint(x+SIZE_OF_IMG_LENGTH, y+SIZE_OF_IMG_WIDTH);
	Point_t x2 = CreatePoint(a, b);
	Point_t y2 = CreatePoint(a + SIZE_OF_IMG_LENGTH, b + SIZE_OF_IMG_WIDTH);
	Point_t s = CreatePoint(x1, y1);
	Point_t z = CreatePoint(x1 + SIZE_OF_IMG_LENGTH, y1 + SIZE_OF_IMG_WIDTH);
	int* imgData =NULL;
	initImg(c, d, imgData);
	initImg(x2, y2, imgData);
	initImg(s, z, imgData);
	//points for range
	Point_t TL = CreatePoint(0, 0);
	Point_t BR = CreatePoint(40, 40);
	int arr[100] = { 0 };
	getImagesIdInRangedByTwoPoints(TL, BR, arr);
	removeData();
	//act
	bool isLoaded = loadImageFromDiskToCache(42, NULL);
	//Assert
	CHECK(isLoaded == false);
	freeQuadTreeAndLinkedListAndArr();

}
TEST_CASE("Cache is asking to load exsiting id ")
{
	//arrange
	initDiskMangmantCb();
	int q= randomValue(0, 38);
	int w= randomValue(0, 38);
	int e= randomValue(0, 38);
	int r= randomValue(0, 38);
	int t= randomValue(0, 38);
	//points for data
	Point_t c = CreatePoint(q, w);
	Point_t d = CreatePoint(q+SIZE_OF_IMG_LENGTH, 3 + SIZE_OF_IMG_WIDTH);
	Point_t x = CreatePoint(w, e);
	Point_t y = CreatePoint(w + SIZE_OF_IMG_LENGTH, e + SIZE_OF_IMG_WIDTH);
	Point_t s = CreatePoint(r, t);
	Point_t z = CreatePoint(r+SIZE_OF_IMG_LENGTH, t + SIZE_OF_IMG_WIDTH);
	int* imgData =NULL;
	initImg(c, d, imgData);
	initImg(x, y, imgData);
	initImg(s, z, imgData);
	//points for range
	Point_t TL = CreatePoint(0, 0);
	Point_t BR = CreatePoint(40, 40);
	ImagePoints_t imageRange = CreateImagePoint(TL, BR);
	int arr[100] = { 0 };
	getImagesIdInRangedByTwoPoints(TL, BR, arr);
	//act
	bool a = loadImageFromDiskToCache(46, NULL);
	//Assert
	CHECK(a == true);
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("remove when disk is full and entering the same point value")
{

	//arrange
	initDiskMangmantCb();
	//creating 20 images
	for (int i = 0; i < 20; i++)
	{
		Point_t c = CreatePoint(i, i);
		Point_t d = CreatePoint(i + 2, i + 2);

		int* imgData =NULL;
		initImg(c, d, imgData);
	}
	//act
	Point_t c = CreatePoint(1, 1);
	Point_t d = CreatePoint(3, 3);
	int* imgData =NULL;
	AddImgToDiskMangmant(c, d, imgData);
	//assert
	CHECK(diskMangmantCb->linkedList->tail->prev->imgInfo->imgPoints.TL.x != 0);
	CHECK(diskMangmantCb->linkedList->head->next->imgInfo->imgPoints.TL.x == 1);
	freeQuadTreeAndLinkedListAndArr();
	

}
TEST_CASE("remove when disk is full and entering  different point value")
{

	//arrange
	initDiskMangmantCb();
	//creating 20 images
	for (int i = 0; i < 20; i++)
	{
		Point_t c = CreatePoint(i, i);
		Point_t d = CreatePoint(i + 2, i + 2);

		int* imgData =NULL;
		initImg(c, d, imgData);
	}
	//act
	Point_t c = CreatePoint(1, 6);
	Point_t d = CreatePoint(3, 8);
	int* imgData =NULL;
	AddImgToDiskMangmant(c, d, imgData);
	//assert
	CHECK(diskMangmantCb->linkedList->tail->prev->imgInfo->imgPoints.TL.x != 0);
	CHECK(diskMangmantCb->linkedList->head->next->imgInfo->imgPoints.TL.x == 1);
	freeQuadTreeAndLinkedListAndArr();
	

}
TEST_CASE("search when the disk is empty")
{
	//Arrange
	initDiskMangmantCb();
	int arr[100] = { 0 };
	Point_t pointTL = CreatePoint(0, 0);
	Point_t pointBR = CreatePoint(15, 15);
	int count = 0;
	//act
	searchImgsAtQuadTreeByRange(diskMangmantCb->quadTree, pointTL, pointBR, &count, arr);

	//Assert
	CHECK(arr[0] == NULL);
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("add an img with uncorrect value") {
	//Arrange
	initDiskMangmantCb();

	int q = randomValue(0, 36);
	int w = randomValue(0, 38);
 
	Point_t pointTL = CreatePoint(q, w);
	Point_t pointBR = CreatePoint(q+SIZE_OF_IMG_LENGTH*2, w+SIZE_OF_IMG_WIDTH);
	int* data=NULL;
	//act
	AddImgToDiskMangmant(pointTL, pointBR, data);
	//Assert
	CHECK(errno == IMG_IS_NOT_IN_CORRECT_SIZE);
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("search when range is bigger than the quadTree range")
{
	//arrenge
	initDiskMangmantCb();
	int q = randomValue(0, 38);
	int w = randomValue(0, 38);
	int e = randomValue(0, 38);
	int r = randomValue(0, 38);
	//creating img info
	Point_t c = CreatePoint(q, w);
	Point_t d = CreatePoint(q + SIZE_OF_IMG_LENGTH, w+ SIZE_OF_IMG_WIDTH);
	Point_t x = CreatePoint(w, q);
	Point_t y = CreatePoint(w + SIZE_OF_IMG_LENGTH, q + SIZE_OF_IMG_WIDTH);
	Point_t s = CreatePoint(e, r);
	Point_t z = CreatePoint(e + SIZE_OF_IMG_LENGTH, r + SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(c, d);
	ImagePoints_t pointB = CreateImagePoint(x, y);
	ImagePoints_t pointC = CreateImagePoint(s, z);
	int* data =NULL;
	//init
	initImg(c, d, data);
	initImg(x, y, data);
	initImg(s, z, data);
	//point range
	Point_t tl = CreatePoint(0, 0);
	Point_t br = CreatePoint(50, 50);
	//
	int arr[100] = { 0 };
	int count = 0;
	//act
	searchImgsAtQuadTreeByRange(diskMangmantCb->quadTree, tl, br, &count, arr);
	//assert
	CHECK(arr[0] != NULL);
	CHECK(arr[1] != NULL);
	CHECK(arr[2] != NULL);
	CHECK(arr[3] == NULL);
	freeQuadTreeAndLinkedListAndArr();
	
}
TEST_CASE("search when range is not in quadTree range ")
{
	//arrenge
	initDiskMangmantCb();
	//creating img info
	int q = randomValue(0, 38);
	int w = randomValue(0, 38);
	int e = randomValue(0, 38);
	int r = randomValue(0, 38);
	//creating img info
	Point_t c = CreatePoint(q, w);
	Point_t d = CreatePoint(q + SIZE_OF_IMG_LENGTH, w + SIZE_OF_IMG_WIDTH);
	Point_t x = CreatePoint(w, q);
	Point_t y = CreatePoint(w + SIZE_OF_IMG_LENGTH, q + SIZE_OF_IMG_WIDTH);
	Point_t s = CreatePoint(e, r);
	Point_t z = CreatePoint(e + SIZE_OF_IMG_LENGTH, r + SIZE_OF_IMG_WIDTH);
	ImagePoints_t point = CreateImagePoint(c, d);
	ImagePoints_t pointB = CreateImagePoint(x, y);
	ImagePoints_t pointC = CreateImagePoint(s, z);
	int* data = NULL;
	//init
	initImg(c, d, data);
	initImg(x, y, data);
	initImg(s, z, data);
	//point range
	int v = randomValue(41, 70);
	int m = randomValue(41, 70);
	int q2 = randomValue(70, 100);
	int w2 = randomValue(70, 100);
	Point_t tl = CreatePoint(v, m);
	Point_t br = CreatePoint(q2, w2);
	//
	int arr[100] = { 0 };
	int count = 0;
	//act
	searchImgsAtQuadTreeByRange(diskMangmantCb->quadTree, tl, br, &count, arr);
	//assert
	CHECK(arr[0] == NULL);
	freeQuadTreeAndLinkedListAndArr();
	
}