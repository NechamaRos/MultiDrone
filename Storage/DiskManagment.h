#pragma once
#pragma once
//#include "DiskManagment.c"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include<errno.h>

#define SIZE_OF_IMG_LENGTH 2
#define SIZE_OF_IMG_WIDTH 2
#define SIZE_OF_CACHE 15
typedef struct Point_s {
    int x;
    int y;
} Point_t;

typedef struct ImagePoints_s {
    Point_t TL;
    Point_t BR;
} ImagePoints_t;

typedef struct ImageInfo_s {
    int imgId;
    ImagePoints_t imgPoints;
    int* disk_ptr;
} ImageInfo_t;

typedef struct UnitNodeLinkedLst_s {
    // int id;
    struct UnitNodeLinkedLst_s* next;
    struct UnitNodeLinkedLst_s* prev;
    ImageInfo_t* imgInfo;//צריך לעשות מצביע???
    struct  QuadNode_s* positionOnTree_ptr;
}UnitNodeLinkedLst_t;

typedef struct LinkedList_s {
    UnitNodeLinkedLst_t* head;
    UnitNodeLinkedLst_t* tail;
    int AmountOfLinks;
} LinkedList_t;

typedef struct QuadNode_s {
    ImagePoints_t imagePoints;
    UnitNodeLinkedLst_t* LinkedList_ptr;
    struct QuadTree_s* parent;
} QuadNode_t;

//The tree is built from a quad tree and a quad node
// so that a quad tree contains the subrange of the space
// and has two options: either the quad node is not equal to NULL or the pointers to the quad tree
// or at least one of the pointers is not equal to NULL

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
    ImageInfo_t* arraySearchInfo[100];
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
ImageInfo_t* CreateImageInfo(ImagePoints_t imgPoints, const char* imgData);
bool isCorrectPoints(Point_t TL, Point_t BR);
bool isCorrectSize(Point_t TL, Point_t BR);
void initImg(Point_t TL, Point_t BR, const char* imgData);
void initDiskMangmantCb();
void printLinkList(FILE* file);
//linkedList
//UnitNodeLinkedLst_t* InsertCreateNode(ImageInfo_t imgInfo, LinkedList_t* linkedList);
UnitNodeLinkedLst_t* createNode(ImageInfo_t* imgInfo);
void insertToLinkList(UnitNodeLinkedLst_t* node);
LinkedList_t* createLinkedList();
void removeData();
UnitNodeLinkedLst_t* removeNodeFromLinkedList();
void removeIfExist(UnitNodeLinkedLst_t* node);
void removeFromdArraySearchInfo(int id);
void moveToTheBeginning(UnitNodeLinkedLst_t* nodePtr);

//QuadTree

QuadNode_t* createQuadNode(ImagePoints_t imagePoint, UnitNodeLinkedLst_t* nodePtr);
void connectBetweenDatStructures(UnitNodeLinkedLst_t* nodePtr, QuadNode_t* quadNode);
QuadTree_t* createQuadTree(Point_t TL, Point_t BR);
void searchImgsAtQuadTreeByRange(QuadTree_t* quadTree, Point_t TL, Point_t BR, int* count, int* idArray);
void insertTotheQuadtree(QuadNode_t* node, QuadTree_t* quadTree);
void printSearchResult(int* length);
bool removeQuadNodeFromTree(QuadNode_t* quadNode);
void printQuadTree(int depth, QuadTree_t* quadTree, FILE* file);
void freeQuadTreeAndLinkedListAndArr();


//
void throwExceptiontoFile(Exception exceptions);
