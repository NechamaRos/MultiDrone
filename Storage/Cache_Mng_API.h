#pragma once
#include "Cache_Mng.h"
//get 2 points from Master , return all the images pointers in this range 

int** cache_getImagesPointersInRangeFromMaster(Point_t topLeft, Point_t buttomRight);

//get 2 points from Prefetch , return the 2 pointes after loading the images in range to cache

void cache_LoadingImagesInRangeToCache(Point_t topLeft, Point_t buttomRight);

//get image id , delete the image from the cache & the cache mapping

void cache_deleteImageByImageId(int imageId);