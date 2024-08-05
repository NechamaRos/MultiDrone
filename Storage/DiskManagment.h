#pragma once
#include"DiskManagmentApi.h"
#include <string.h>
#include <stdio.h>
#include<errno.h>


//The initialization variables can change as needed
#define SIZE_OF_IMG_LENGTH 2
#define SIZE_OF_IMG_WIDTH 2
#define SIZE_OF_CACHE 15



typedef struct ImagePoints_s {
    Point_t TL;
    Point_t BR;
} ImagePoints_t;

typedef struct ImageInfo_s {
    int imgId;
    ImagePoints_t imgPoints;
    int* disk_ptr;
} ImageInfo_t;


//one node of the linked List that contains next, prev 
//and all the img info and a ptr to tne place in the quad tree
typedef struct UnitNodeLinkedList_s {
    struct UnitNodeLinkedList_s* next;
    struct UnitNodeLinkedList_s* prev;
    ImageInfo_t* imgInfo;
    struct  QuadNode_s* positionOnTree_ptr;
}UnitNodeLinkedList_t;

typedef struct LinkedList_s {
    UnitNodeLinkedList_t* head;
    UnitNodeLinkedList_t* tail;
    int AmountOfLinks;
} LinkedList_t;

typedef struct QuadNode_s {
    ImagePoints_t imagePoints;
    UnitNodeLinkedList_t* LinkedList_ptr;
    struct QuadTree_s* parent;
} QuadNode_t;

//The tree is built from a quad tree and a quad node
// so that a quad tree contains the subrange of the space
// and has two options: either the quad node is not equal to NULL 
// or at least one of the pointers to the quad tree  is not equal to NULL

typedef struct QuadTree_s {
    Point_t TL;
    Point_t BR;
    struct QuadTree_s* topLeftTree;
    struct QuadTree_s* topRightTree;
    struct QuadTree_s* botLeftTree;
    struct QuadTree_s* botRightTree;
    QuadNode_t* quadNode;
    struct QuadTree_s* parent;
    int numOfNonNULLQuadTrees;
} QuadTree_t;

typedef struct DiskMangmantStructCb_s {
    QuadTree_t* quadTree;
    LinkedList_t* linkedList;
    ImageInfo_t* arraySearchInfo[SIZE_OF_CACHE];
    int lengthOfArraySearchInfo;

} DiskMangmantCb_t;

typedef enum {
    POINT_NEGATIVE_ERORR,
    POINT_NOT_IN_RANGE_ERORR,
    ALLOCATE_ERROR,
    IMG_IS_NOT_IN_CORRECT_SIZE
}Exception;

//init function
Point_t CreatePoint(int x, int y);
ImagePoints_t CreateImagePoint(Point_t TL, Point_t BR);
ImageInfo_t* CreateImageInfo(ImagePoints_t imgPoints, int* imgData);
bool isCorrectPoints(Point_t TL, Point_t BR);
bool isCorrectSize(Point_t TL, Point_t BR);
void initImg(Point_t TL, Point_t BR, int* imgData);
void initDiskMangmantCb();

//linkedList
UnitNodeLinkedList_t* createNode(ImageInfo_t* imgInfo);
void insertToLinkList(UnitNodeLinkedList_t* node);
LinkedList_t* createLinkedList();
void removeData();
UnitNodeLinkedList_t* removeNodeFromLinkedList();
void removeIfExist(UnitNodeLinkedList_t* node);
void removeFromdArraySearchInfo(int id);
void moveToTheBeginning(UnitNodeLinkedList_t* nodePtr);

//QuadTree

QuadNode_t* createQuadNode(ImagePoints_t imagePoint, UnitNodeLinkedList_t* nodePtr);
void connectBetweenDatStructures(UnitNodeLinkedList_t* nodePtr, QuadNode_t* quadNode);
QuadTree_t* createQuadTree(Point_t TL, Point_t BR);
void searchImgsAtQuadTreeByRange(QuadTree_t* quadTree, Point_t TL, Point_t BR, int* count, int* idArray);
void insertTotheQuadtree(QuadNode_t* node, QuadTree_t* quadTree);
bool removeQuadNodeFromTree(QuadNode_t* quadNode);
void freeQuadTreeAndLinkedListAndArr();


//
void throwExceptiontoFile(Exception exceptions);
