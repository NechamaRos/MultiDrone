#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DISK_SIZE 1000

// Forward declarations

typedef struct StackNode_s StackNode_t;
typedef struct DiskFreeIndexesInArray_s DiskFreeIndexesInArray_t;
typedef struct Disk_Management_CB_s Disk_Managment_CB_t;

// Enum declaration
typedef enum {
    Error_When_Allocating_Memory_Space

}Exception;

// Struct declarations

struct StackNode_s {
    int freeIndex;
    StackNode_t* next;
};

struct DiskFreeIndexesInArray_s {
    StackNode_t* top;
};
struct Disk_Management_CB_s {
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

void stack_initialize();

bool stack_is_empty();

void stack_push(StackNode_t* new_node);

StackNode_t* stackNode_create(int index);

int stack_pop();

int stack_top();

