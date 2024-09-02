#pragma once
#include"DiskManagment.h"
//Api implementeded
void saveTheQuadTreeToDisk(QuadTree_t* pointerToQuadTreeTable);
void saveTheLinkedListToDisk(LinkedList_t* pointerToLinkedListTable);
bool loadImageToCache(int* diskAddress, int* cachAddress);
void deleteImageFromDisk(int* diskAdress);
int* addImgToDisk(Point_t TL, Point_t BR, char* imgData, int imgId);
void wirteItWasTrunedOn(int num);
int readingIsThisTheFirstTimeToTurnOn();
QuadTree_t* loadTheQuadTree(void);
LinkedList_t* loadTheLinkedlist(void);
void printLinkList(FILE* file);
void printQuadTree(int depth, QuadTree_t* quadTree, FILE* file);
