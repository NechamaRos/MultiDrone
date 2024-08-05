
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
    stack_initialize();

}
void stack_initialize() {
    disk_mng_CB->diskFreeIndexesInArray=(DiskFreeIndexesInArray_t*)allocate_memory(sizeof(DiskFreeIndexesInArray_t), "Failed to allocate memory for stack ", "stack_initialize");
    disk_mng_CB->diskFreeIndexesInArray->top = NULL;

    //API call if its first time or ot
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
    disk_mng_CB->diskFreeIndexesInArray->top = new_node;
}

int stack_pop() {
    if (stack_is_empty(disk_mng_CB->diskFreeIndexesInArray)) {
        return -1;
    }
    StackNode_t* temp = disk_mng_CB->diskFreeIndexesInArray->top;
    int value = temp->freeIndex;
    disk_mng_CB->diskFreeIndexesInArray->top = disk_mng_CB->diskFreeIndexesInArray->top->next;
    free(temp);
    return value;
}

int stack_top(DiskFreeIndexesInArray_t* stack) {
    if (stack_is_empty(stack)) {
        return -1;
    }
    return stack->top->freeIndex;
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
