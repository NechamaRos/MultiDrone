#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <cstdlib>
#include <ctime>

extern "C" {
#include "Disk_Mng_Master.h",
}

struct GlobalSetup {
	GlobalSetup() {
		srand(time(0));
	}
};
GlobalSetup globalSetup;

int generateRandomNumber(int upper= DISK_SIZE) {
	int lower = 0;
	return (rand() % (upper - lower + 1)) + lower;
}


//initialize 

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