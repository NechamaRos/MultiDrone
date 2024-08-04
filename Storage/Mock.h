#pragma once
#include"DiskManagment.h"
//Api implementeded
void saveTheQuadTreeToDisk_Api(QuadTree_t* pointerToQuadTreeTable);
void saveTheLinkedListToDisk_Api(LinkedList_t* pointerToLinkedListTable);
bool loadImageToCache_Api(int* diskAddress, int* cachAddress);
void DeleteImageFromDisk_Api(int* diskAdress);
int* AddImgToDisk_Api(Point_t TL, Point_t BR, char* imgData, int imgId);
void wirteFlushFlag(int num);
int readFlushFlag();
QuadTree_t* LoadTheQuadTree_Api();
LinkedList_t* LoadTheLinkedlist_Api();
void printLinkList(FILE* file);
void printQuadTree(int depth, QuadTree_t* quadTree, FILE* file);
