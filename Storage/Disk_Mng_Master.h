#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DISK_SIZE 1000

// Forward declarations

typedef struct Point_s Point_t;
typedef struct MapRange_s MapRange_t;
typedef struct ArrayInfo_s ArrayInfo_t;
typedef struct StackNode_s StackNode_t;
typedef struct DiskFreeIndexesInArray_s DiskFreeIndexesInArray_t;
typedef struct Disk_Management_CB_s Disk_Managment_CB_t;

// Enum declaration
typedef enum {
    Error_When_Allocating_Memory_Space

}Exception;

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
};
extern Disk_Managment_CB_t* disk_mng_CB;

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

