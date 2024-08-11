#include "Disk_Mng_Master.h"
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

 Disk_Management_CB_t* disk_mng_CB;

void disk_mng_initialize_CB()
{
    disk_mng_CB = (Disk_Management_CB_t*)allocate_memory(sizeof(Disk_Management_CB_t), "Failed to allocate memory for disk control block", "disk_mng_initialize_CB");
}

void disk_mng_initialize()
{
    disk_mng_initialize_CB();
    stack_firstInitialize();
    array_firstInitialize();
    avlTree_firstInitialize();
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
    StackNode_t* new_node = (StackNode_t*)allocate_memory(sizeof(StackNode_t), "Failed to allocate memory for max heap", "stackNode_create");
    new_node->freeIndex = index;
    new_node->next = disk_mng_CB->diskFreeIndexesInArray->top;
    return new_node;
}


void stack_push(int index) 
{
    StackNode_t* new_node = stackNode_create(index);
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

int disk_mng_deleteMapFromDiskManagementDataStructures(int sizeToFree)
{
    AVLNodeInfo_t* nodeToDelete = avlTree_FindingTheNodeThatIsSuitableForDeletion(disk_mng_CB->disk_SortByMapSize->root);//find the suitable map to delete
    disk_mng_CB->disk_SortByMapSize->root = avlTree_deleteNode(disk_mng_CB->disk_SortByMapSize->root,nodeToDelete);//delete the map from the AVL tree
    ArrayInfo_t* arrayInfo = disk_mng_CB->arrayForAllMApsInformation[nodeToDelete->arrayIndex];//finding the object that corresponds to deletion in the array
    bool isDeleteSuccess=disk_deleteMap(arrayInfo->diskPointer);//send API to the disk to delete the map
    if (isDeleteSuccess)
    {
        cache_deleteMap(arrayInfo->mapid);//send API to the cache to delete the map
        stack_push(nodeToDelete->arrayIndex);//push the index to the stack of free indexes
        sizeToFree -= arrayInfo->size;//reducing the size that needs to be deleted
        array_deleteFromArray(nodeToDelete->arrayIndex);//delete the map from the array
        array_deleteArrayInfo(arrayInfo);//delete the array object
        avlNode_delete(nodeToDelete);//delete the avl node
    }
    else
    {
        test_writeExceptionToFile(Error_When_Deleting_Map_from_Disk,"disk_mng_deleteMapFromDiskManagementDataStructures");
    }
}

void disk_mng_delete(int mapSize)
{
    while (mapSize>0)
    {
        mapSize = disk_mng_deleteMapFromDiskManagementDataStructures(mapSize);
    }
}

//AVL node info
AVLNodeInfo_t* avlNodeInfo_create(int mapSize, int arrayIndex)
{
    AVLNodeInfo_t* newAVLNodeInfo = (AVLNodeInfo_t*)allocate_memory(sizeof(AVLNodeInfo_t), "Failed to allocate memory for AVL node info", "avlNodeInfo_create");
    newAVLNodeInfo->mapSize = mapSize;
    newAVLNodeInfo->arrayIndex = arrayIndex;
    newAVLNodeInfo->lru = disk_mng_CB->disk_SortByMapSize->lruCounter+1;
    return newAVLNodeInfo;
}

void avlNodeInfo_delete(AVLNodeInfo_t* nodeInfo)
{
    if (nodeInfo != NULL) {
        free(nodeInfo);
    }
}

//AVL node 
AVLNode_t* avlNode_create(AVLNodeInfo_t* avlNodeInfo) {
    AVLNode_t* node = (AVLNode_t*)allocate_memory(sizeof(AVLNode_t), "Failed to allocate memory for AVL node", "avlTree_insert");
    node->avlNodeInfo = avlNodeInfo;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}
void avlNode_delete(AVLNode_t* node) {
    if (node != NULL) {
        avlNodeInfo_delete(node->avlNodeInfo);//?
        free(node);
    }
}

int avlNode_height(AVLNode_t* N) {
    if (N == NULL)
        return 0;
    return N->height;
}



AVLNode_t* avlTree_rightRotate(AVLNode_t* y) {
    AVLNode_t* x = y->left;
    AVLNode_t* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(avlNode_height(y->left), avlNode_height(y->right)) + 1;
    x->height = max(avlNode_height(x->left), avlNode_height(x->right)) + 1;
    return x;
}

AVLNode_t* avlTree_leftRotate(AVLNode_t* x) {
    AVLNode_t* y = x->right;
    AVLNode_t* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(avlNode_height(x->left), avlNode_height(x->right)) + 1;
    y->height = max(avlNode_height(y->left), avlNode_height(y->right)) + 1;
    return y;
}

int avlTree_getBalance(AVLNode_t* N) {
    if (N == NULL)
        return 0;
    return avlNode_height(N->left) - avlNode_height(N->right);
}
AVLNode_t* avlTree_minValueNode(AVLNode_t* node) {
    AVLNode_t* current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}
AVLNode_t* avlTree_insert(AVLNode_t* node, AVLNode_t* newNode) {
    if (node == NULL)
    {
        return newNode;
    }

    if (newNode->avlNodeInfo->mapSize < node->avlNodeInfo->mapSize)
        node->left = avlTree_insert(node->left, newNode);
    else
        node->right = avlTree_insert(node->right, newNode);

    node->height = 1 + max(avlNode_height(node->left), avlNode_height(node->right));

    int balance = avlTree_getBalance(node);

    if (balance > 1 && newNode->avlNodeInfo->mapSize < node->left->avlNodeInfo->mapSize)
        return avlTree_rightRotate(node);

    if (balance < -1 && newNode->avlNodeInfo->mapSize > node->right->avlNodeInfo->mapSize)
        return avlTree_leftRotate(node);

    if (balance > 1 && newNode->avlNodeInfo->mapSize > node->left->avlNodeInfo->mapSize) {
        node->left = avlTree_leftRotate(node->left);
        return avlTree_rightRotate(node);
    }

    if (balance < -1 && newNode->avlNodeInfo->mapSize < node->right->avlNodeInfo->mapSize) {
        node->right = avlTree_rightRotate(node->right);
        return avlTree_leftRotate(node);
    }

    return node;
}

void avlTree_firstInitialize() {
    disk_mng_CB->disk_SortByMapSize = (DiskSortByMapSize_t*)allocate_memory(sizeof(DiskSortByMapSize_t), "Failed to allocate memory for AVL Tree", "avlTree_initialize");
    disk_mng_CB->disk_SortByMapSize->root = NULL;
    disk_mng_CB->disk_SortByMapSize->totalElements = 0;
    disk_mng_CB->disk_SortByMapSize->lruCounter = 0;
}

void avlTree_insertElement(AVLNodeInfo_t* nodeInfo) {
    disk_mng_CB->disk_SortByMapSize->lruCounter++;
    AVLNode_t* newNode = avlNode_create(nodeInfo);
    disk_mng_CB->disk_SortByMapSize->root = avlTree_insert(disk_mng_CB->disk_SortByMapSize->root, newNode);
    disk_mng_CB->disk_SortByMapSize->totalElements++;
}


AVLNode_t* avlTree_FindingTheNodeThatIsSuitableForDeletion(AVLNode_t* node) {
    if (node == NULL) {
        return NULL;
    }

    AVLNode_t* eligibleNode = NULL;
    AVLNode_t* largestNode = node;

    // Check the right subtree first for the largest node
    AVLNode_t* rightResult = avlTree_FindingTheNodeThatIsSuitableForDeletion(node->right);
    if (rightResult != NULL) {
        if (rightResult->avlNodeInfo->lru <= disk_mng_CB->disk_SortByMapSize->lruCounter * 0.7) {
            if (eligibleNode == NULL || rightResult->avlNodeInfo->mapSize > (eligibleNode ? eligibleNode->avlNodeInfo->mapSize : 0)) {
                eligibleNode = rightResult;
            }
        }
        if (rightResult->avlNodeInfo->mapSize > largestNode->avlNodeInfo->mapSize) {
            largestNode = rightResult;
        }
    }

    // Check if the current node is eligible for deletion
    if (node->avlNodeInfo->lru <= disk_mng_CB->disk_SortByMapSize->lruCounter * 0.7) {
        if (eligibleNode == NULL || node->avlNodeInfo->mapSize > eligibleNode->avlNodeInfo->mapSize) {
            eligibleNode = node;
        }
    }
    if (node->avlNodeInfo->mapSize > largestNode->avlNodeInfo->mapSize) {
        largestNode = node;
    }

    // Check the left subtree if no eligible node found in the right subtree or current node
    AVLNode_t* leftResult = avlTree_FindingTheNodeThatIsSuitableForDeletion(node->left);
    if (leftResult != NULL) {
        if (leftResult->avlNodeInfo->lru <= disk_mng_CB->disk_SortByMapSize->lruCounter * 0.7) {
            if (eligibleNode == NULL || leftResult->avlNodeInfo->mapSize > eligibleNode->avlNodeInfo->mapSize) {
                eligibleNode = leftResult;
            }
        }
        if (leftResult->avlNodeInfo->mapSize > largestNode->avlNodeInfo->mapSize) {
            largestNode = leftResult;
        }
    }

    // Return the eligible node if found, otherwise return the node with the largest mapSize
    return eligibleNode ? eligibleNode : largestNode;
}
AVLNode_t* avlTree_deleteNode(AVLNode_t* root, AVLNode_t* node) {
    if (root == NULL) {
        return root;
    }

    // STEP 1: PERFORM STANDARD BST DELETE
    if (node->avlNodeInfo->mapSize < root->avlNodeInfo->mapSize) {
        root->left = avlTree_deleteNode(root->left, node);
    }
    else if (node->avlNodeInfo->mapSize > root->avlNodeInfo->mapSize) {
        root->right = avlTree_deleteNode(root->right, node);
    }
    else {
        // This is the node to be deleted
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode_t* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL) {
                // Simply remove the current node
                root = NULL;
            }
            else {
                // One child case
                // Update root to point to the non-empty child
                *root = *temp;
                // Make sure to set the child pointers of the old node to NULL
                root->left = temp->left;
                root->right = temp->right;
            }
        }
        else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            AVLNode_t* temp = avlTree_minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->avlNodeInfo = temp->avlNodeInfo;

            // Delete the inorder successor
            root->right = avlTree_deleteNode(root->right, temp);
        }
    }

    // If the tree had only one node then return
    if (root == NULL) {
        return root;
    }

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(avlNode_height(root->left), avlNode_height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether this node became unbalanced)
    int balance = avlTree_getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && avlTree_getBalance(root->left) >= 0) {
        return avlTree_rightRotate(root);
    }

    // Left Right Case
    if (balance > 1 && avlTree_getBalance(root->left) < 0) {
        root->left = avlTree_leftRotate(root->left);
        return avlTree_rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && avlTree_getBalance(root->right) <= 0) {
        return avlTree_leftRotate(root);
    }

    // Right Left Case
    if (balance < -1 && avlTree_getBalance(root->right) > 0) {
        root->right = avlTree_rightRotate(root->right);
        return avlTree_leftRotate(root);
    }

    return root;
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
