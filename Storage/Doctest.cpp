#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <cstdlib>
#include <ctime>
#include <stdbool.h>


extern "C" {
#include "Disk_Mng_Master.h"
#include "Disk_Mng_Master_API.h"
}
struct GlobalSetup {
    GlobalSetup() {
        srand(time(0));
    }
};
GlobalSetup globalSetup;

int generateRandomNumber(int upper = DISK_SIZE) {
    int lower = 0;
    return (rand() % (upper - lower + 1)) + lower;
}
int index, popValue,value;

//mock functions

//the function get pointer to map in the disk and delete this map from disk
void disk_deleteMap(int* diskPointer) {}


//the function fill in the structer all the data which save befoe the computer closed,the function get  destenation,suorce,length;
void disk_loadDataForInitializeDataStructers(void* i,void* x,void* b) 
{
}

TEST_CASE("test_disk_mng_initialize_CB")
{
    disk_mng_initialize_CB();
    CHECK(disk_mng_CB != NULL);
}

TEST_CASE("test_disk_mng_initialize")
{
	disk_mng_initialize();
}

TEST_CASE("test_avlNodeInfo_create")
{
	int mapSize = generateRandomNumber();
	int indexArray = generateRandomNumber();
	int lru = generateRandomNumber();

	AVLNodeInfo_t* avlNodeInfo = avlNodeInfo_create(mapSize, indexArray, lru);

	CHECK(avlNodeInfo->mapSize == mapSize);
	CHECK(avlNodeInfo->arrayIndex == indexArray);
	CHECK(avlNodeInfo->lru == lru);
}

TEST_CASE("test_avlNodeInfo_delete")
{
	int mapSize = generateRandomNumber();
	int indexArray = generateRandomNumber();
	int lru = generateRandomNumber();

	AVLNodeInfo_t* avlNodeInfo = avlNodeInfo_create(mapSize, indexArray, lru);
	avlNodeInfo_delete(avlNodeInfo);
}

TEST_CASE("test_avlNode_create")
{
	int mapSize = generateRandomNumber();
	int indexArray = generateRandomNumber();
	int lru = generateRandomNumber();

	AVLNodeInfo_t* avlNodeInfo = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode = avlNode_create(avlNodeInfo);

	CHECK(avlNode->avlNodeInfo == avlNodeInfo);
	CHECK(avlNode->height == 1);
	CHECK(avlNode->left  == NULL);
	CHECK(avlNode->right == NULL);
}

TEST_CASE("test_avlNode_delete")
{
	int mapSize = generateRandomNumber();
	int indexArray = generateRandomNumber();
	int lru = generateRandomNumber();

	AVLNodeInfo_t* avlNodeInfo = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode = avlNode_create(avlNodeInfo);
	avlNode_delete(avlNode);
}

TEST_CASE("test_avlNode_height")
{
	int mapSize = generateRandomNumber();
	int indexArray = generateRandomNumber();
	int lru = generateRandomNumber();

	AVLNodeInfo_t* avlNodeInfo = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode = avlNode_create(avlNodeInfo);

	CHECK(avlNode->height == avlNode_height(avlNode));
}

TEST_CASE("test_avlTree_rightRotate")
{
	int mapSize = generateRandomNumber();
	int indexArray = generateRandomNumber();
	int lru = generateRandomNumber();

	AVLNodeInfo_t* avlNodeInfo1 = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode1 = avlNode_create(avlNodeInfo1);

	AVLNodeInfo_t* avlNodeInfo2 = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode2 = avlNode_create(avlNodeInfo2);

	AVLNodeInfo_t* avlNodeInfo3 = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode3 = avlNode_create(avlNodeInfo3);

	avlNode1->left = avlNode2;
	avlNode2->left = avlNode3;

	avlNode2->height = std::max(avlNode_height(avlNode2->left), avlNode_height(avlNode2->right)) + 1;
	avlNode1->height = std::max(avlNode_height(avlNode1->left), avlNode_height(avlNode1->right)) + 1;

	AVLNode_t* newRoot = avlTree_rightRotate(avlNode1);

	CHECK(newRoot == avlNode2); 
	CHECK(newRoot->right == avlNode1); 
	CHECK(newRoot->left == avlNode3); 
	CHECK(avlNode1->left == nullptr);
	CHECK(avlNode1->height == 1); 
	CHECK(newRoot->height == 2);
}

TEST_CASE("test_avlTree_leftRotate")
{
	int mapSize = generateRandomNumber();
	int indexArray = generateRandomNumber();
	int lru = generateRandomNumber();

	AVLNodeInfo_t* avlNodeInfo1 = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode1 = avlNode_create(avlNodeInfo1);

	AVLNodeInfo_t* avlNodeInfo2 = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode2 = avlNode_create(avlNodeInfo2);

	AVLNodeInfo_t* avlNodeInfo3 = avlNodeInfo_create(mapSize, indexArray, lru);
	AVLNode_t* avlNode3 = avlNode_create(avlNodeInfo3);



	avlNode1->right = avlNode2;
	avlNode2->right = avlNode3;

	avlNode2->height = std::max(avlNode_height(avlNode2->left), avlNode_height(avlNode2->right)) + 1;
	avlNode1->height = std::max(avlNode_height(avlNode1->left), avlNode_height(avlNode1->right)) + 1;

	AVLNode_t* newRoot = avlTree_leftRotate(avlNode1);

	CHECK(newRoot == avlNode2); 
	CHECK(newRoot->left == avlNode1); 
	CHECK(newRoot->right == avlNode3); 
	CHECK(avlNode1->right == nullptr); 
	CHECK(avlNode1->height == 1); 
	CHECK(newRoot->height == 2);
}

//stack tests

TEST_CASE("test_stack_firstInitialize")
{
    disk_mng_initialize();
    for (int i = 0; i < DISK_SIZE; i++)
    {
        StackNode_t* newNode = stackNode_create(i);
        stack_push(newNode);
    }

    for (int i =DISK_SIZE-1; i >0; i--)
    {
        CHECK( stack_pop() == i);
    }
}


TEST_CASE("test_stack_normalInitialize")
{
    disk_mng_initialize();
}

TEST_CASE("test_stack_push")
{
    disk_mng_initialize();
    for (size_t i = 0; i < DISK_SIZE; i++)
    {  
    index = generateRandomNumber();
    StackNode_t* newStackNode= stackNode_create(index);
    stack_push(newStackNode);
    popValue = stack_pop();
    CHECK(popValue == index);
    }

}
TEST_CASE("test_stack_pop")
{
    disk_mng_initialize();
    for (size_t i = 4; i < 0; i++)
    {
        value = stack_pop();
        CHECK(value !=stack_top());
    }
}
TEST_CASE("test_stack_top")
{
    disk_mng_initialize();
    for (size_t i = DISK_SIZE-1; i < 0; i--)
    {
        value = stack_top();
        stack_pop();
        CHECK(value == i);
    }
}
TEST_CASE("test_stack_is_empty")
{
    disk_mng_initialize();
    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        stack_pop();
    }
    CHECK(stack_is_empty());

}

TEST_CASE("test_stackNode_create")
{
    int index = generateRandomNumber();
    StackNode_t* new_node = stackNode_create(index);
    CHECK(new_node->freeIndex == index);
    CHECK(new_node->next == disk_mng_CB->diskFreeIndexesInArray->top);
}

//array tests

TEST_CASE("test_array_deleteFromArray")
{
    int index = generateRandomNumber();
    array_deleteFromArray(index);
    CHECK(disk_mng_CB->arrayForAllMApsInformation[index]== NULL);
}

TEST_CASE("test_arrayInfo_create")
{
    int mapid = generateRandomNumber();;
    int * diskPointer = NULL;
    int size = generateRandomNumber();
    Point_t topLeft;
    topLeft.x = generateRandomNumber();
    topLeft.y = generateRandomNumber();
    Point_t bottomRight;
    bottomRight.x = generateRandomNumber();
    bottomRight.y = generateRandomNumber();
    MapRange_t* mapRange = mapRange_create(bottomRight, topLeft);


    //arrayInfo->avlInfo = avlInfo;
    ArrayInfo_t* arrayInfo= arrayInfo_create(mapid, diskPointer, size, mapRange);
    CHECK(arrayInfo->mapid == mapid);
    CHECK(arrayInfo->diskPointer == NULL);
    CHECK(arrayInfo->size == size);
    CHECK(arrayInfo->range == mapRange);
}

//range functions
TEST_CASE("test_mapRange_create")
{
    Point_t topLeft;
    topLeft.x = generateRandomNumber();
    topLeft.y = generateRandomNumber();
    Point_t bottomRight;
    bottomRight.x = generateRandomNumber();
    bottomRight.y = generateRandomNumber();
    MapRange_t* mapRange = mapRange_create(bottomRight, topLeft);
    CHECK(mapRange->bottomRight.x == bottomRight.x);
    CHECK(mapRange->bottomRight.y == bottomRight.y);
    CHECK(mapRange->topLeft.x == topLeft.x);
    CHECK(mapRange->topLeft.y == topLeft.y);
}

