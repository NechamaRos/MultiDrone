#pragma once
#include "PointForSlave.h"
static int reqIdStatic = 0;
class mapRequest
{
private:
	PointForSlave TL;
	PointForSlave BR;
	int reqId;
public:
	mapRequest()
	{
		//Top Left : 31.8200° N, 35.1766° E
		//Bottom Right : 31.7961° N, 35.2075° E
		PointForSlave tl(31.8200, 35.1766);
		PointForSlave br(31.7961, 35.2075);
		this->TL = tl;
		this->BR = br;
		this->reqId = reqIdStatic++;
	}
	PointForSlave GetBR()
	{
		return this->BR;
	}
	PointForSlave GetTL()
	{
		return this->TL;
	}
	int getReqId()
	{
		return this->reqId;
	}
};
