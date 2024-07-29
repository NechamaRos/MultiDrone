#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define CACHE_SIZE 100
// Forward declarations
typedef struct ImageInfo_s ImageInfo_t;
typedef struct LinkedListNode_s LinkedListNode_t;
typedef struct HashInfo_s HashInfo_t;
typedef struct QueueNode_s QueueNode_t;
typedef struct CacheEmptyAddresses_s CacheEmptyAddresses_t;
typedef struct Point_s Point_t;
typedef struct Cache_Management_CB_s Cache_Management_CB_t;
typedef struct CacheLRU_s CacheLRU_t;
typedef struct CacheByImageId_s CacheByImageId_t;

// Enum declaration
typedef enum {
    Failed_To_Load_Image_From_Disk_To_Cache,
    Error_When_Allocating_Memory_Space
}Exception;

typedef enum {
    MASTER_CALL,
    PREFETCH_CALL,
    PREFETCH_AND_MASTER_CALL
} CallSource;

// Struct declarations
struct Point_s {
    int x; // longitude
    int y; // latitude
};

struct LinkedListNode_s {
    ImageInfo_t* imageInfo;
    LinkedListNode_t* prev;
    LinkedListNode_t* next;
};

struct CacheLRU_s {
    LinkedListNode_t* head;
    LinkedListNode_t* tail;
    int size;
};

struct HashInfo_s {
    int imageId;
    LinkedListNode_t* linkPointer;
    HashInfo_t* next;
};

struct ImageInfo_s {
    int imageId;
    HashInfo_t* hashPointer;
    int* cachePointer;
};

struct CacheByImageId_s {
    HashInfo_t** entries;
    size_t size;
    size_t length;
};

struct QueueNode_s {
    int* address;
    QueueNode_t* next;
};

struct CacheEmptyAddresses_s {
    QueueNode_t* front;
    QueueNode_t* rear;
    int size;
};

struct Cache_Management_CB_s {
    CacheLRU_t* cache_LRU;
    CacheByImageId_t* cache_ByImageId;
    CacheEmptyAddresses_t* cache_EmptyAddresses;
    CallSource sourceForAPICall;
    int** ram;
};

extern Cache_Management_CB_t* cache_mng_CB;

//mock functions for doctests

void test_writeExceptionToFile(Exception exception, char* source);

int disk_Mng_getImagesIdInRangedByTwoPoints(Point_t topLeft, Point_t bottomRight, int* arrayOfImagesId, int cacheSize);

bool disk_Mng_loadImageFromDiskToCache(int imageId, int* address);

bool prefetch_IsTheImageInPrefetchLoading(Point_t topLeft, Point_t bottomRight);

void prefetch_InsertNewRangeToPrefetch(Point_t topLeft, Point_t bottomRight);

void prefetch_removeRangeFromRangesInLoadingArray(Point_t topLeft, Point_t bottomRight);


//cache managment

//initilaize the Ram
void ram_initialize();

//cache_getRange_internal-get 2 points ,Fetches image pointers in the given range from cache or loads them from disk if not present.
int ** cache_getRange_internal(Point_t topLeft, Point_t bottomRight);

//cache_TreatmentOfReturningAnswers-get images pointers in range,returns to each the requested answer
int ** cache_TreatmentOfReturningAnswers(Point_t topLeft, Point_t bottomRight,int** imagePointersInCache);

//cache_deleteImageFromCacheMapping-get linkedListNode pointer,delete the image from the data structers of the cache mapping

void cache_deleteImageFromCacheMapping(LinkedListNode_t* node);

//cache_deleteImageByLRU-delete 10% from the chache according to LRU
void cache_deleteImageByLRU();

//cache_addImageToTheCacheMapping-add new image to the cache mapping
HashInfo_t* cache_addImageToTheCacheMapping(int imageId);

//initialize the cache CB
void cache_initialize_CB();

//initialize the cache mapping
void cache_initialize();

//allocate_memory-generic function to allocate memory for all the objects
void* allocate_memory(size_t size, const char* description, const char* functionName);

//isContinuInLoop- checking whether to continue the loop
bool isContinuInLoop(int** imagePointersInCache, int indexInArray, int counter);

//cache_addImagePointerToTheArray-add pointer to image in cache to array
void cache_addImagePointerToTheArray(int** imagePointersInCache, int indexInArray, HashInfo_t* hashInfo);




//Hash table- save the images in cache by image ID avg Runtime access O(1)
 
//initialize Hash table key by image id
void hashTable_initialize();

//hashing  table 
 int hashTable_function(int imageId);

//insert object to the hash table 
void hashTable_insert(HashInfo_t* hashInfo);

//search hashInfo by imageId in the hash table 
HashInfo_t* hashTable_search(int imageId);

//delete hashInfo from the hash table
void hashTable_delete(HashInfo_t* hashInfo);

//DoublyLinkedList - Sorting by LRU (least recently used) for highly efficient deletion,
//each link contains pointers to the other objects in the other data structures to link them

//initilaize linked list sort by LRU
void linkedList_initialize();

//insert new node to the head of the linked list
void linkedList_insert(LinkedListNode_t* node);

//move object to the head of the linked list
void linkedList_moveToHead(LinkedListNode_t* node);

//delete node from the linked list
void linkedList_deleteNode(LinkedListNode_t* node);



//linked list Node functions

//create new node 
LinkedListNode_t* node_create();

//delete node
void node_delete(LinkedListNode_t* node);

//ImageInfo for  functions

void imageInfo_delete(ImageInfo_t* imageInfo);

//create new nodeInfo 
ImageInfo_t* imageInfo_create(int imageId,HashInfo_t* hashPointer,int* cachePointer);

//HashInfo functions

//create new hashInfo 
HashInfo_t* hashInfo_create(int imageId, LinkedListNode_t* linkPointer);

//delete  hash object
void hashInfo_delete(HashInfo_t* hashInfo);

//Queue - save the empty addresses in chache

//initilaize queue
void queue_initialize();

//initilaize the queue with all the addresses of the cache
void queue_initilaizeWithEmptyAddresses();

//create new queueNode for the queue
QueueNode_t* queue_createQueueNode(int* address);

//check if the queue empty-the cache full
bool queue_isEmpty();

//insert node to the queue
void queue_enqueue(int* address);

//delete node from the queue
int* queue_dequeue();

//returning the first address in the queue
int* queue_top();






void printHashTable(CacheByImageId_t* hashTable);
