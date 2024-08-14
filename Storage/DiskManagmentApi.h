#pragma once
#include<stdbool.h>
typedef struct Point_s {
    int x;
    int y;
} Point_t;

//gets a imgId of a img to load and a address 
//and loads the img data to address
bool loadImageFromDiskToCache(int imgId, int* addrassToLoading);

//gets a range by tl ,br and return all the img ids in the range in the arr that was sent  
int getImagesIdInRangedByTwoPoints(Point_t topLeft, Point_t bottomRight, int* arrayOfImagesId);

//gets img info and adds the img to the disk and to the data struct
void addImgToDiskMangmant(Point_t TL, Point_t BR, int* imgData);

//save the data struct before shutdown
void saveBeforeShutdown();

//Boot when the device is turned on
void bootWhenTheDeviceIsTurnedOn();

//init the data structs
void initDiskMangmantCb();




