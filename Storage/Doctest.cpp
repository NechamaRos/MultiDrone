#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <cstdlib>
#include <ctime>

extern "C"{
#include "Cache_Mng.h",
#include "Cache_Mng_API.h"
}

struct GlobalSetup {
    GlobalSetup() {
        srand(time(0)); 
    }
};
GlobalSetup globalSetup;

int generateRandomNumber(int upper) {
    int lower = 0;
    return (rand() % (upper - lower + 1)) + lower;
}


//mock functions

int disk_Mng_getImagesIdInRangedByTwoPoints(Point_t topLeft, Point_t bottomRight, int* arrayOfImagesId, int cacheSize)
{
    int countOfImagesInRange = generateRandomNumber(100);
    for (int i = 0; i < countOfImagesInRange; i++) {

        int imageId= generateRandomNumber(10000000);
        arrayOfImagesId[i] = imageId;
    }
    return countOfImagesInRange;
}

bool disk_Mng_loadImageFromDiskToCache(int imageId, int* address) 
{

    if (imageId%2==0)
    {
        return true;
    }
    return false;
}

bool prefetch_IsTheImageInPrefetchLoading(Point_t topLeft, Point_t bottomRight)
{
    //To check if the function works we will use even and odd to decide if the image is loaded or not
    int num = generateRandomNumber(10000000);
    if (num % 2 == 0)
    {
        return true;
    }
    return false;
}
void prefetch_InsertNewRangeToPrefetch(Point_t topLeft, Point_t bottomRight) 
{
    //mock function insert new range to prefetch
}
void prefetch_removeRangeFromRangesInLoadingArray(Point_t topLeft, Point_t bottomRight)
{
    //mock function remove range from ranges in loading array
}

//initialize  - what to do to check if an assignment was actually made (there is such a check in the function)
TEST_CASE("test_queue_initilaizeWithEmptyAddresses")
{
    cache_initialize_CB();
    ram_initialize();
    queue_initialize();
    queue_initilaizeWithEmptyAddresses();
    CHECK(cache_mng_CB->cache_EmptyAddresses->front != NULL);
    CHECK(cache_mng_CB->cache_EmptyAddresses->rear != NULL);
    CHECK(cache_mng_CB->cache_EmptyAddresses->size == CACHE_SIZE);
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        CHECK(queue_dequeue() == cache_mng_CB->ram[i]);
    }
}
TEST_CASE("test_linkedList_initialize")
{
    cache_initialize_CB();
    linkedList_initialize();
    CHECK(cache_mng_CB->cache_LRU->head == NULL);
    CHECK(cache_mng_CB->cache_LRU->tail == NULL);
    CHECK(cache_mng_CB->cache_LRU->size == 0);
}
TEST_CASE("test_queue_initialize")
{
    cache_initialize_CB();
    queue_initialize();
    CHECK(cache_mng_CB->cache_EmptyAddresses->front == NULL);
    CHECK(cache_mng_CB->cache_EmptyAddresses->rear == NULL);
    CHECK(cache_mng_CB->cache_EmptyAddresses->size == 0);
}
TEST_CASE("test_hashTable_initialize")
{
    cache_initialize_CB();
    hashTable_initialize();
    CHECK(cache_mng_CB->cache_ByImageId->length == 0);
    CHECK(cache_mng_CB->cache_ByImageId->size == CACHE_SIZE);
}
TEST_CASE("test_ram_initialize")
{
    cache_initialize_CB();
    ram_initialize();
}
TEST_CASE("test_initialize_CB")
{
    cache_initialize_CB();
}
TEST_CASE("test_cache_initialize")
{
    cache_initialize();
}


//create
TEST_CASE("test_linklistNode_create")
{
    LinkedListNode_t* newNode = node_create();
    CHECK(newNode->imageInfo == NULL);
    CHECK(newNode->prev == NULL);
    CHECK(newNode->next == NULL);
}
TEST_CASE("test_hashInfo_create")
{
    int imageId = generateRandomNumber(10000000);
    LinkedListNode_t* linkPointer = node_create();
    HashInfo_t* hashInfo = hashInfo_create(imageId,linkPointer);
    CHECK(hashInfo->imageId == imageId);
    CHECK(hashInfo->linkPointer == linkPointer);
    CHECK(hashInfo->next == NULL);
}
TEST_CASE("test_imageInfo_create")
{
    cache_initialize_CB();
    ram_initialize();
    queue_initialize();
    queue_initilaizeWithEmptyAddresses();

    int imageId = generateRandomNumber(10000000);
    LinkedListNode_t* linkPointer = node_create();
    HashInfo_t* hashPointer = hashInfo_create(imageId, linkPointer);
    int* cachePointer = queue_dequeue();

    ImageInfo_t* imageInfo = imageInfo_create(imageId, hashPointer, cachePointer);
    CHECK(imageInfo->hashPointer == hashPointer);
    CHECK(imageInfo->cachePointer == cachePointer);
    CHECK(imageInfo->imageId == imageId);
}
TEST_CASE("test_queue_createQueueNode")
{
    cache_initialize_CB();
    ram_initialize();
    queue_initialize();
    queue_initilaizeWithEmptyAddresses();
    int* address = queue_dequeue();

    QueueNode_t* newQueueNode = queue_createQueueNode(address);
    CHECK(newQueueNode->address == address);
    CHECK(newQueueNode->next == NULL);
}

//insert
TEST_CASE("test_linkedList_insert")
{
    cache_initialize_CB();
    linkedList_initialize();

    for (int i = 0; i < CACHE_SIZE ; i++)
    {
        LinkedListNode_t* node = node_create();
        linkedList_insert(node);
        CHECK(cache_mng_CB->cache_LRU->head == node);
    }
    CHECK(cache_mng_CB->cache_LRU->size == CACHE_SIZE);
}
TEST_CASE("test_hashTable_insert")
{
    cache_initialize_CB();
    hashTable_initialize();
    int imageId;
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        imageId = generateRandomNumber(10000000);
        while (hashTable_search(imageId)!=NULL)
        {
            imageId = generateRandomNumber(10000000);
        }
        LinkedListNode_t* linkPointer = node_create();
        HashInfo_t* hashInfo = hashInfo_create(imageId, linkPointer);
        hashTable_insert(hashInfo);
        CHECK(hashTable_search(imageId) == hashInfo);
    }
    printf("insert\n");
    printHashTable(cache_mng_CB->cache_ByImageId);
    CHECK(cache_mng_CB->cache_ByImageId->length == CACHE_SIZE);
}
TEST_CASE("test_queue_enqueue")
{
    cache_initialize_CB();
    queue_initialize();
    ram_initialize();

    for (int i = 0; i < CACHE_SIZE; i++)
    {
        queue_enqueue(cache_mng_CB->ram[i]);
    }
    CHECK(cache_mng_CB->cache_EmptyAddresses->size == CACHE_SIZE);
}
//delete
TEST_CASE("test_linkedList_deleteNode")
{
    cache_initialize_CB();
    linkedList_initialize();

    for (int i = 0; i < CACHE_SIZE; i++)
    {
        LinkedListNode_t* node = node_create();
        linkedList_insert(node);
    }
    //delete tail
    LinkedListNode_t* tail = cache_mng_CB->cache_LRU->tail;
    linkedList_deleteNode(cache_mng_CB->cache_LRU->tail);
    CHECK(cache_mng_CB->cache_LRU->tail != tail);

    //delete head
    LinkedListNode_t* head = cache_mng_CB->cache_LRU->head;
    linkedList_deleteNode(cache_mng_CB->cache_LRU->head);
    CHECK(cache_mng_CB->cache_LRU->head != head);

    //delete middle
    LinkedListNode_t* middle = cache_mng_CB->cache_LRU->head->next;
    linkedList_deleteNode(cache_mng_CB->cache_LRU->head->next);
    CHECK(cache_mng_CB->cache_LRU->head->next != middle);
}
TEST_CASE("test_hashTable_delete")
{
    cache_initialize_CB();
    hashTable_initialize();

    for (int i = 1; i <=CACHE_SIZE; i++)
    {
        int imageId = i;
        LinkedListNode_t* linkPointer = node_create();
        HashInfo_t* hashInfo = hashInfo_create(imageId, linkPointer);
        hashTable_insert(hashInfo);
    }
    printf("delete\n");

    printHashTable(cache_mng_CB->cache_ByImageId);

    for (int i = 1; i <= CACHE_SIZE; i++)
    {
        hashTable_delete(hashTable_search(i));
        CHECK(hashTable_search(i) == NULL);
    }
    printf("after delete\n");
    printHashTable(cache_mng_CB->cache_ByImageId);

}
TEST_CASE("test_queue_dequeue")
{
    cache_initialize_CB();
    ram_initialize();
    queue_initialize();
    queue_initilaizeWithEmptyAddresses();

    for (int i = 0; i < CACHE_SIZE; i++)
    {
        queue_dequeue();
    }
    CHECK(queue_isEmpty());
}

TEST_CASE("test_cache_deleteImageByLRU")
{
    cache_initialize();
    int imageId;
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        imageId = generateRandomNumber(10000000);
        while (hashTable_search(imageId) != NULL)
        {
            imageId = generateRandomNumber(10000000);
        }
        cache_addImageToTheCacheMapping(imageId);
    }
    printf("LRU\n");

    printHashTable(cache_mng_CB->cache_ByImageId);

    cache_deleteImageByLRU();
    CHECK(cache_mng_CB->cache_LRU->size == CACHE_SIZE - CACHE_SIZE / 10);
    printf("----------------------after LRU----------------------\n");

    printHashTable(cache_mng_CB->cache_ByImageId);

}
TEST_CASE("test_cache_deleteImageByImageId")
{
    cache_initialize();
    int imageId;
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        imageId = generateRandomNumber(10000000);
        while (hashTable_search(imageId) != NULL)
        {
            imageId = generateRandomNumber(10000000);
        }
        cache_addImageToTheCacheMapping(imageId);
        cache_deleteImageByImageId(imageId);
        CHECK(hashTable_search(imageId) == NULL);
    }
}
//move-linked list
TEST_CASE("test_linkedList_moveToHead")
{
    cache_initialize();

    for (int i = 1; i <= CACHE_SIZE; i++)
    {
        int imageId =i;
        cache_addImageToTheCacheMapping(imageId);
    }
    printf("move\n");

    printHashTable(cache_mng_CB->cache_ByImageId);

    int imageId = generateRandomNumber(10000000);
    HashInfo_t* hashInfo = hashTable_search(imageId);
    if (hashInfo!=NULL)
    {
        linkedList_moveToHead(hashInfo->linkPointer);
        CHECK(cache_mng_CB->cache_LRU->head == hashInfo->linkPointer);
    }
}
//isEmpty-queue
TEST_CASE("test_queue_isEmpty")
{
    cache_initialize_CB();
    ram_initialize();
    queue_initialize();

    int index = generateRandomNumber(100);
    int* address = cache_mng_CB->ram[index];
    queue_enqueue(address);
    CHECK(!queue_isEmpty());
    queue_dequeue();
    CHECK(queue_isEmpty());
}

//search-hash table
TEST_CASE("test_hashTable_search")
{
    cache_initialize_CB();
    hashTable_initialize();

    int imageId = generateRandomNumber(10000000);
    LinkedListNode_t* linkPointer = node_create();
    HashInfo_t* hashInfo = hashInfo_create(imageId, linkPointer);
    hashTable_insert(hashInfo);

   CHECK(hashTable_search(imageId)==hashInfo);
}
TEST_CASE("test_hashTable_function")
{
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        int imageId = generateRandomNumber(10000000);
        CHECK(hashTable_function(imageId) == imageId % CACHE_SIZE);
    }
}
//cache
 

//TEST_CASE("test_cache_deleteImageFromCacheMapping")

TEST_CASE("test_cache_getRange_internal")
{
    Point_t topLeft;
    topLeft.x = generateRandomNumber(10000000);
    topLeft.y = generateRandomNumber(10000000);

    Point_t bottomRight;
    bottomRight.x = generateRandomNumber(10000000);
    bottomRight.y = generateRandomNumber(10000000);

    cache_initialize();
    int imageId;
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        imageId = generateRandomNumber(10000000);
        while (hashTable_search(imageId) != NULL)
        {
            imageId = generateRandomNumber(10000000);
        }
        cache_addImageToTheCacheMapping(imageId);
    }
    int**imagesPointers= cache_getRange_internal(topLeft, bottomRight);

}
TEST_CASE("test_cache_LoadingImagesInRangeToCache")
{
    Point_t topLeft;
    topLeft.x = generateRandomNumber(10000000);
    topLeft.y = generateRandomNumber(10000000);

    Point_t bottomRight;
    bottomRight.x = generateRandomNumber(10000000);
    bottomRight.y = generateRandomNumber(10000000);

    cache_initialize();
    int imageId;
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        imageId = generateRandomNumber(10000000);
        while (hashTable_search(imageId) != NULL)
        {
            imageId = generateRandomNumber(10000000);
        }
        cache_addImageToTheCacheMapping(imageId);
    }
    cache_LoadingImagesInRangeToCache(topLeft,bottomRight);
}
TEST_CASE("test_cache_getImagesPointersInRangeFromMaster")
{
    Point_t topLeft;
    topLeft.x = generateRandomNumber(10000000);
    topLeft.y = generateRandomNumber(10000000);

    Point_t bottomRight;
    bottomRight.x = generateRandomNumber(10000000);
    bottomRight.y = generateRandomNumber(10000000);

    cache_initialize();
    int imageId;
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        imageId = generateRandomNumber(10000000);
        while (hashTable_search(imageId) != NULL)
        {
            imageId = generateRandomNumber(10000000);
        }
        cache_addImageToTheCacheMapping(imageId);
    }
    int** imagesPointers = cache_getImagesPointersInRangeFromMaster(topLeft, bottomRight);

}
//TEST_CASE("test_cache_TreatmentOfReturningAnswers")
//TEST_CASE("test_cache_loadImageFromDiskToCacheAndInsertToTheCacheMapping")

