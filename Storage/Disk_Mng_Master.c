
#include "Disk_Mng_Master.h"
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

 Disk_Managment_CB_t* disk_mng_CB;

void disk_mng_initialize_CB()
{
    disk_mng_CB = (Disk_Managment_CB_t*)allocate_memory(sizeof(Disk_Managment_CB_t), "Failed to allocate memory for disk control block", "disk_mng_initialize_CB");
}

void disk_mng_initialize()
{
    disk_mng_initialize_CB();
    stack_firstInitialize();
    array_firstInitialize();
}
//initialize on the first time we turn o the computer the stack will initialize with all the indexes.
void stack_firstInitialize() {
    disk_mng_CB->diskFreeIndexesInArray=(DiskFreeIndexesInArray_t*)allocate_memory(sizeof(DiskFreeIndexesInArray_t), "Failed to allocate memory for stack ", "stack_firstInitialize");
    disk_mng_CB->diskFreeIndexesInArray->top = NULL;
    disk_mng_CB->diskFreeIndexesInArray->size = 0;
    for (size_t i = 0; i < DISK_SIZE; i++)
    {
        StackNode_t* s= stackNode_create(i);
        stack_push(s);
    }
}

void stack_normalInitialize() {
    disk_mng_CB->diskFreeIndexesInArray = (DiskFreeIndexesInArray_t*)allocate_memory(sizeof(DiskFreeIndexesInArray_t), "Failed to allocate memory for stack ", "stack_normalInitialize");
    int length = 0;
    //disk_loadDataForInitializeDataStructers(length, 0, sizeof(int));
    //disk_loadDataForInitializeDataStructers(disk_mng_CB->diskFreeIndexesInArray, 4, length*sizeof(int));
}

bool stack_is_empty() {
    return  disk_mng_CB->diskFreeIndexesInArray->top == NULL;
}

StackNode_t* stackNode_create(int index)
{   
    StackNode_t* new_node = (StackNode_t*)allocate_memory(sizeof(StackNode_t), "Failed to allocate memory for max heap", "stack_push");
    new_node->freeIndex = index;
    new_node->next = disk_mng_CB->diskFreeIndexesInArray->top;
    return new_node;
}


void stack_push(StackNode_t* new_node) 
{

    disk_mng_CB->diskFreeIndexesInArray->size++;
    disk_mng_CB->diskFreeIndexesInArray->top = new_node;
}

int stack_pop() {
    if (stack_is_empty()) {
        return -1;
    }
    StackNode_t* temp = disk_mng_CB->diskFreeIndexesInArray->top;
    int value = temp->freeIndex;
    disk_mng_CB->diskFreeIndexesInArray->top = disk_mng_CB->diskFreeIndexesInArray->top->next;
    disk_mng_CB->diskFreeIndexesInArray->size--;
    free(temp);
    return value;
}

int stack_top() {
    if (stack_is_empty()) {
        return -1;
    }
    return disk_mng_CB->diskFreeIndexesInArray->top->freeIndex;
}

//array fuctions

void array_firstInitialize() {
    disk_mng_CB->arrayForAllMApsInformation = (ArrayInfo_t**)allocate_memory(DISK_SIZE*sizeof(ArrayInfo_t*), "Failed to allocate memory for stack ", "array_firstInitialize");
}

void array_normalInitialize() {
    disk_mng_CB->diskFreeIndexesInArray = (ArrayInfo_t**)allocate_memory(sizeof(ArrayInfo_t*), "Failed to allocate memory for stack ", "array_normalInitialize");
    //disk_loadDataForInitializeDataStructers(disk_mng_CB->diskFreeIndexesInArray, 4+ disk_mng_CB->diskFreeIndexesInArray->size* sizeof(int),DISK_SIZE*sizeof(ArrayInfo_t**));
}

ArrayInfo_t* arrayInfo_create(int mapid, int* diskPointer, int size, MapRange_t* range
    //,AVLNodeInfo_t*  avlInfo//
)
{
    ArrayInfo_t* arrayInfo = (ArrayInfo_t*)allocate_memory(sizeof(ArrayInfo_t), "Failed to allocate memory for max heap", "arrayInfo_create");
    arrayInfo->mapid = mapid;
    arrayInfo ->diskPointer = diskPointer;
    arrayInfo->size = size;
    arrayInfo->range =range;
    //arrayInfo->avlInfo = avlInfo;
    return arrayInfo;
}

void array_deleteFromArray(int index)
{
    disk_mng_CB->arrayForAllMApsInformation[index] = NULL;
}

void array_deleteArrayInfo(ArrayInfo_t* arrayInfo)
{
    disk_deleteMap(arrayInfo->diskPointer);
    free(arrayInfo);
}

//maprange functions

MapRange_t* mapRange_create(Point_t bottomRight, Point_t topLeft) 
{
    MapRange_t* mapRange = (MapRange_t*)allocate_memory(sizeof(MapRange_t), "Failed to allocate memory for max heap", "mapRange_create");
    mapRange->bottomRight = bottomRight;
    mapRange->topLeft = topLeft;
    return mapRange;
}




void test_writeExceptionToFile(Exception exception, const char* source) {
    FILE* file;
    errno_t err = fopen_s(&file, "errors.log.txt", "a");
    if (err != 0) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    const char* error_message;
    switch (exception) {
    case Error_When_Allocating_Memory_Space:
        error_message = "Error: Error when allocating memory space.";
        break;
    default:
        error_message = "Error: Unknown exception.";
        break;
    }

    fprintf(file, "%s:\n%s\n", source, error_message);
    fclose(file);
}

void* allocate_memory(size_t size, const char* description, const char* functionName) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        test_writeExceptionToFile(Error_When_Allocating_Memory_Space, functionName);
        perror(description);
        return NULL;
    }
    return ptr;
}
