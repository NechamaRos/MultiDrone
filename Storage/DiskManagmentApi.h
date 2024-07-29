#pragma once
#include"DiskManagment.h"

bool disk_Mng_loadImageFromDiskToCache_Api(int id, int* addrassToLoading);
int disk_Mng_getImagesIdInRangedByTwoPoints_Api(Point_t topLeft, Point_t bottomRight, int* arrayOfImagesId);
void AddImgToDiskMangmant_Api(Point_t TL, Point_t BR, const char* imgData);
void saveBeforeShutdown();
void bootWhenTheDeviceIsTurnedOn();




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
