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
 
// allocate_memory-Allocates memory and logs the allocation with a description and function name
void* allocate_memory(size_t size, const char* description, const char* functionName);

// test_writeExceptionToFile-Writes exception details to a file
void test_writeExceptionToFile(Exception exception, const char* source);

//initialize

// disk_mng_initialize_CB-Initializes the control block for disk management
void disk_mng_initialize_CB();

// disk_mng_initialize-Initializes the disk management system
void disk_mng_initialize();

//AVL node info

// avlNodeInfo_create-Creates and returns a new AVLNodeInfo_t structure
AVLNodeInfo_t* avlNodeInfo_create(int mapSize, int arrayIndex, int lru);

// avlNodeInfo_delete-Deletes an AVLNodeInfo_t structure
void avlNodeInfo_delete(AVLNodeInfo_t* nodeInfo);

//AVL node

// avlNode_create-Creates and returns a new AVLNode_t structure
AVLNode_t* avlNode_create(AVLNodeInfo_t* avlNodeInfo);

// avlNode_delete-Deletes an AVLNode_t structure
void avlNode_delete(AVLNode_t* node);

// avlNode_height-Returns the height of a given AVLNode_t
int avlNode_height(AVLNode_t* N);

//AVL tree

// avlTree_rightRotate-Performs a right rotation on the given subtree
AVLNode_t* avlTree_rightRotate(AVLNode_t* y);

// avlTree_leftRotate-Performs a left rotation on the given subtree
AVLNode_t* avlTree_leftRotate(AVLNode_t* x);

// avlTree_getBalance-Returns the balance factor of the given AVLNode_t
int avlTree_getBalance(AVLNode_t* N);

// avlTree_minValueNode-Finds and returns the node with the smallest value in the tree
AVLNode_t* avlTree_minValueNode(AVLNode_t* node);

// avlTree_insert-Inserts a new node into the AVL tree
AVLNode_t* avlTree_insert(AVLNode_t* node, AVLNodeInfo_t* data, int lruCounter);

// avlTree_firstInitialize-Initializes the AVL tree for the first time
void avlTree_firstInitialize();

// avlTree_insertElement-Inserts a new element into the AVL tree
void avlTree_insertElement(AVLNodeInfo_t* data);

// avlTree_FindingTheNodeThatIsSuitableForDeletion-Finds the node suitable for deletion according to conditions
AVLNode_t* avlTree_FindingTheNodeThatIsSuitableForDeletion(AVLNode_t* node);

// avlTree_deleteNode-Deletes a node from the AVL tree without freeing its memory
AVLNode_t* avlTree_deleteNode(AVLNode_t* root, AVLNode_t* node);
