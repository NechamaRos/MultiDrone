#pragma once
#pragma once
#include "ImageFromSlave.h"
#include "MessageParams.h"

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
	STM_SEND_IMAGES_PROP_PARAMS GetAStructForImageInPoint(MTS_ASK_FOR_IMAGES_PROP_PARAMS msg) {};
	STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS return_of_an_image_by_its_properties(MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS msgR) {};
};









