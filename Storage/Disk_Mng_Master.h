#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "Disk_Mng_Master_API.h"

#define POINT_BR_RANGE (Point_t){100, 100}
#define POINT_TL_RANGE (Point_t){0, 0}
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
    Error_When_Allocating_Memory_Space,
    Error_When_Deleting_Map_from_Disk,
    Error_When_Adding_Map_To_Disk,
    Error_Worng_Size_Variable,
    Error_Worng_Map_Variable,
    Error_Worng_Map_Range
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
    AVLNodeInfo_t* avlNodeInfo;
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
    int mapIdIndex;
    ArrayInfo_t** arrayForAllMApsInformation;
    DiskFreeIndexesInArray_t* diskFreeIndexesInArray;
    DiskSortByMapSize_t* disk_SortByMapSize;
};
extern Disk_Management_CB_t* disk_mng_CB;
void printTree(AVLNode_t* root);
void printNode(AVLNode_t* node);



// Function declarations

// allocate_memory-Allocates memory and logs the allocation with a description and function name
void* allocate_memory(size_t size, const char* description, const char* functionName);

// test_writeExceptionToFile-Writes exception details to a file
void test_writeExceptionToFile(Exception exception, const char* source);

//disk_mng_DeleteMapFromDiskManagementDataStructures- delete map from the disk
int disk_mng_deleteMapFromDiskManagementDataStructures(int sizeToFree);

//disk_mng_delete-Deleting maps until there is enough space to add a new map with the resulting size to disk
void disk_mng_delete(int mapSize);

//disk_mng_checkDataStructures-check 
bool disk_mng_isTheDataCorrect(MapRange_t* range, int size, int* map);


//initialize

// disk_mng_initialize_CB-Initializes the control block for disk management
void disk_mng_initialize_CB();

// disk_mng_initialize-Initializes the disk management system
void disk_mng_initialize();

// disk_mng_firstInitialize-Initializes the disk management system at the first time
void disk_mng_firstInitialize();

// disk_mng_normalInitialize-Initializes the disk management system each time the computer opened
void disk_mng_normalInitialize();


//AVL node info

// avlNodeInfo_create-Creates and returns a new AVLNodeInfo_t structure
AVLNodeInfo_t* avlNodeInfo_create(int mapSize, int arrayIndex);

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
// avlTree_firstInitialize-Initializes the AVL tree for the first time
void avlTree_firstInitialize();

// avlTree_firstInitialize-Initializes the AVL tree each time the computer opened
void avlTree_normalInitialize();

//save data from avlTree each closed
void avlTree_saveData();

// avlTree_rightRotate-Performs a right rotation on the given subtree
AVLNode_t* avlTree_rightRotate(AVLNode_t* y);

// avlTree_leftRotate-Performs a left rotation on the given subtree
AVLNode_t* avlTree_leftRotate(AVLNode_t* x);

// avlTree_getBalance-Returns the balance factor of the given AVLNode_t
int avlTree_getBalance(AVLNode_t* N);

// avlTree_minValueNode-Finds and returns the node with the smallest value in the tree
AVLNode_t* avlTree_minValueNode(AVLNode_t* node);

// avlTree_firstInitialize-Initializes the AVL tree for the first time
void avlTree_firstInitialize();

// avlTree_insert-Inserts a new node into the AVL tree
AVLNode_t* avlTree_insert(AVLNode_t* node, AVLNode_t* newNode);

// avlTree_insertElement-Inserts a new element into the AVL tree
void avlTree_insertElement(AVLNodeInfo_t* nodeInfo);


// avlTree_FindingTheNodeThatIsSuitableForDeletion-Finds the node suitable for deletion according to conditions
AVLNode_t* avlTree_FindingTheNodeThatIsSuitableForDeletion(AVLNode_t* node);

// avlTree_deleteNode-Deletes a node from the AVL tree without freeing its memory
AVLNode_t* avlTree_deleteNode(AVLNode_t* root, AVLNode_t* node);

//stack functions

//initialize on the first time when we turn on the computer-fill in the stack all the index from 0-DISK_SIZE
void stack_firstInitialize();

//normal initialize each open of the computer the disk will fill in the structers all the saved data
void stack_normalInitialize();

//save data from stack each closed
void stack_saveData();

//stack_is_empty check if the stack full
bool stack_is_empty();


// Push a new node into the stack
void stack_push(int index);

// Create a new stack node with the given index value
StackNode_t* stackNode_create(int index);

// Pop the top element from the stack
int stack_pop();

// Get the value of the top element of the stack without popping it
int stack_top();


//array functions 

//initialize on the first time when we turn on the computer just allocate memory to the array
void array_firstInitialize();

// Initializes an array with default values or in a standard way.
// This function likely sets the array to a known starting state.
void array_normalInitialize();

//save data from array each closed
void array_saveData();

// Deletes an element from an array at a specific index.
void array_deleteFromArray(int index);

// Frees or deletes information associated with an array.
void array_deleteArrayInfo(ArrayInfo_t* arrayInfo);

//disk_deleteMap the function get pointer to map in the disk and delete this map from disk
bool disk_deleteMap(int* diskPointer);

void cache_deleteMap(int mapId);


//create a new arrayInfo with all the parameters
ArrayInfo_t* arrayInfo_create(int* diskPointer, int size, MapRange_t* range,AVLNodeInfo_t* avlNodeInfo);

void array_addToArray(ArrayInfo_t* arrayInfo, int index);

//create a new range with 2 point which given
MapRange_t* mapRange_create(Point_t bottomRight, Point_t topLeft);

//disk_loadDataForInitializeDataStructers the function get destination where to load and from where and how many to load
void disk_loadDataForInitializeDataStructers(void* destination, void* startAddress, void* howManyToLoad);

//disk_loadDataForInitializeDataStructers the function get data where to save and from where and how many to save
void disk_saveDataFromStructersToDisk(void* data, void* startAddress, void* howManyToLoad);

void disk_mng_addMapToDiskManagementDataStructures(MapRange_t* range, int size, int* diskPointer);

int* disk_addMap(int* map);

void disk_mng_addMap(MapRange_t* range, int size, int* map);

bool disk_isThereEnoughSpace(int mapSize);

bool isCorrectRange(MapRange_t* range);
