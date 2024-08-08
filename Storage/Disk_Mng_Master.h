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
typedef struct Point_s Point_t;
typedef struct MapRange_s MapRange_t;
typedef struct ArrayInfo_s ArrayInfo_t;
typedef struct StackNode_s StackNode_t;
typedef struct DiskFreeIndexesInArray_s DiskFreeIndexesInArray_t;
typedef struct Disk_Management_CB_s Disk_Management_CB_t;

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

// Struct declarations

struct Point_s {
    int x; // longitude
    int y; // latitude
};

struct MapRange_s {
    Point_t topLeft; 
    Point_t bottomRight; 
};

struct ArrayInfo_s {
    int mapid;
    int* diskPointer;
    int size;
    MapRange_t* range;
    //AVLNodeInfo_t  avlInfo;

};

struct StackNode_s {
    int freeIndex;
    StackNode_t* next;
};

struct DiskFreeIndexesInArray_s {
    int size;
    StackNode_t* top;
};
struct Disk_Management_CB_s {
    ArrayInfo_t** arrayForAllMApsInformation;
    StackNode_t* stackNode;
    DiskFreeIndexesInArray_t* diskFreeIndexesInArray;
    DiskSortByMapSize_t* disk_SortByMapSize;
};
extern Disk_Management_CB_t* disk_mng_CB;

//the function aloocate memory to the object and check if fail
void* allocate_memory(size_t size, const char* description, const char* functionName);

//in case the allocate fail this function write to logFile
void test_writeExceptionToFile(Exception exception, char* source);

//disk managment 

//initialize the disk managment CB

void disk_mng_initialize_CB();

////initialize the disk managment mapping
void disk_mng_initialize();

//stack functions

//initialize on the first time when we turn on the computer-fill in the stack all the index from 0-DISK_SIZE
void stack_firstInitialize();

//normal initialize each open of the computer the disk will fill in the structers all the saved data
void stack_normalInitialize();

//stack_is_empty check if the stack full
bool stack_is_empty();


// Push a new node onto the stack
void stack_push(StackNode_t* new_node);

// Create a new stack node with the given index value
StackNode_t* stackNode_create(int index);

// Pop the top element from the stack
int stack_pop();

// Get the value of the top element of the stack without popping it
int stack_top();


//array functions 


// Initializes an array with default values or in a standard way.
// This function likely sets the array to a known starting state.
void array_normalInitialize();

//initialize on the first time when we turn on the computer just allocate memory to the array
void array_firstInitialize();

// Deletes an element from an array at a specific index.
void array_deleteFromArray(int index);

// Frees or deletes information associated with an array.
void array_deleteArrayInfo(ArrayInfo_t* arrayInfo);


//disk_deleteMap the function get pointer to map in the disk and delete this map from disk
void disk_deleteMap(int* diskPointer);

ArrayInfo_t* arrayInfo_create(int mapid,int* diskPointer,int size,MapRange_t* range
//,AVLNodeInfo_t*  avlInfo//
 );

MapRange_t* mapRange_create(Point_t bottomRight, Point_t topLeft);

