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

void* allocate_memory(size_t size, const char* description, const char* functionName);

void test_writeExceptionToFile(Exception exception, char* source);

//disk managment 

//initialize the disk managment CB

void disk_mng_initialize_CB();

//initialize the cache mapping
void disk_mng_initialize();

//stack functions

void stack_firstInitialize();

void stack_normalInitialize();

bool stack_is_empty();

void stack_push(StackNode_t* new_node);

StackNode_t* stackNode_create(int index);

int stack_pop();

int stack_top();

//array functions 

void array_normalInitialize();

void array_firstInitialize();

void array_deleteFromArray(int index);

void array_deleteArrayInfo(ArrayInfo_t* arrayInfo);

void disk_deleteMap(int* diskPointer);

ArrayInfo_t* arrayInfo_create(int mapid,int* diskPointer,int size,MapRange_t* range
//,AVLNodeInfo_t*  avlInfo//
 );

MapRange_t* mapRange_create(Point_t bottomRight, Point_t topLeft);

