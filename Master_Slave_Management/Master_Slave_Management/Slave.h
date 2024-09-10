#pragma once
#include "ImageFromSlave.h"
#include "Structs.h"
#include <vector>
#include "Message.h"

using namespace std;

class Slave
{
private:
	int id;
	static int Counter;
public:
	Slave()
	{
		Counter++;
		this->id = Counter;
	};
	int GetId()
	{
		return this->id;
	};
	int GetSlaveId()
	{
		return Counter;
	}
	void setId(int id)
	{
		this->id = id;

	}

	vector<ImageFromSlave> CheckIfIHaveImage(PointForSlave p, ImageFromSlave MyArrOfImages[]);
	bool isPointInImage(PointForSlave p, ImageFromSlave img);
	void GetAStructForImageInPoint(Message* message);//STM_SEND_IMAGE_PROP

	void return_of_an_image_by_its_properties(Message* message);


};

