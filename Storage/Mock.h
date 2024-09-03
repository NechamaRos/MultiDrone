#pragma once
typedef struct Point_s Point_t;
typedef struct SlaveImgInfo_s SlaveImgInfo_t;

struct Point_s
{
	int x;
	int y;

};

struct SlaveImgInfo_s
{
	int imgId;
	int slaveId;
	Point_t TL;
	Point_t BR;
	int* data;
};


SlaveImgInfo_t* createSlaveImgInfo(int imgId, int slaveId, Point_t TL, Point_t BR);