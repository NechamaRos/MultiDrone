#pragma once
#include "PointForSlave.h"
class ImageFromSlave
{
private:
	int id;
	static int counterForImage;
	PointForSlave TL;
	PointForSlave BR;
	int slaveId;
	int ReqId;
public:
	ImageFromSlave() {};
	ImageFromSlave(float top, float left, float bottom, float right, int slavenum, int
		reqId) :
		BR(bottom, right),
		TL(top, left),
		slaveId(slavenum),
		ReqId(reqId),
		id(counterForImage++) {};
	PointForSlave GetBR()
	{
		return this->BR;
	}
	PointForSlave GetTL()
	{
		return this->TL;
	}
	int GetSlaveId()
	{
		return this->slaveId;
	};
	int GetReqId()
	{
		return this->ReqId;
	}
	int GetId()
	{
		return this->id;
	}
	int getCounter()
	{
		return this->counterForImage;
	}
	void setSlaveId(int id)
	{
		this->slaveId = id;
	}
};