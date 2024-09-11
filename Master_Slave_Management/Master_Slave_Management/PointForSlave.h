#pragma once

class PointForSlave
{
private:
	float x;
	float y;
public:
	PointForSlave() :x(0), y(0) {};
	PointForSlave(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	float GetX()
	{
		return this->x;
	}
	float GetY()
	{
		return this->y;
	}
};