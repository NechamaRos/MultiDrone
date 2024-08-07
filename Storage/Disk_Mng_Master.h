#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define DISK_SIZE 1000

// Forward declarations
typedef struct AVLNode_s AVLNode_t;
typedef struct DiskSortByMapSize_s DiskSortByMapSize_t;
typedef struct AVLNodeInfo_s AVLNodeInfo_t;
typedef struct Disk_Managment_CB_s Disk_Managment_CB_t;

// Enum declaration
typedef enum {
    Error_When_Allocating_Memory_Space
} Exception;

// Struct declarations
struct AVLNodeInfo_s {
    int mapSize;
    int arrayIndex;
    int lru;
};

struct AVLNode_s {
    AVLNodeInfo_t* avlNodeInfo;
    AVLNode_t* left, * right;
    int height;
};

struct DiskSortByMapSize_s {
    AVLNode_t* root;
    int totalElements;
    int lruCounter; 
};

struct Disk_Managment_CB_s {
    DiskSortByMapSize_t* disk_SortByMapSize;
};

extern Disk_Managment_CB_t* disk_mng_CB;


// Function declarations
 
void* allocate_memory(size_t size, const char* description, const char* functionName);
void test_writeExceptionToFile(Exception exception, const char* source);
void disk_mng_initialize_CB();
void disk_mng_initialize();

AVLNodeInfo_t* avlNodeInfo_create(int mapSize, int arrayIndex, int lru);
void avlNodeInfo_delete(AVLNodeInfo_t* nodeInfo);

AVLNode_t* avlNode_create(AVLNodeInfo_t* avlNodeInfo);
void avlNode_delete(AVLNode_t* node);

int height(AVLNode_t* N);
AVLNode_t* rightRotate(AVLNode_t* y);
AVLNode_t* leftRotate(AVLNode_t* x);
int getBalance(AVLNode_t* N);
AVLNode_t* avlTree_minValueNode(AVLNode_t* node);

AVLNode_t* avlTree_insert(AVLNode_t* node, AVLNodeInfo_t* data, int lruCounter);
void avlTree_firstInitialize();
void avlTree_insertElement(AVLNodeInfo_t* data);

AVLNode_t* findEligibleForDeletion(AVLNode_t* node);
AVLNode_t* avlTree_deleteNode(AVLNode_t* root, AVLNode_t* node);