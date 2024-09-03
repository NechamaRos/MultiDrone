#include <vector>
#include <ctime>
#include "Message.h"
#include "Priority.h"
//#include "Slave.h"
#include "event.h"
#include "MessageType.h"
//#include "RealImage.h"
#include "mapRequest.h"
using namespace std;
typedef struct M_GOT_REQ_PARAMS {
	mapRequest mp;
	//MESSAGE_TYPE:M_GOT_REQ
	//handling function: SlavesManager::getARequestForAMap
}M_GOT_REQ_PARAMS;
typedef struct MTS_ASK_FOR_IMAGES_PARAMS {
	PointForSlave pfs;
	int slaveId;
	int reqId;
	//MESSAGE_TYPE:MTS_ASK_FOR_IMAGES
	//handling function: SlavesManager::sendRequestForImageInSpecificPoint
}MTS_ASK_FOR_IMAGES_PARAMS;
typedef struct STM_SEND_IMAGES_PROP_PARAMS
{
	bool HaveOrNot_Image;
	vector<ImageFromSlave> imgRes = {};
	//MESSAGE_TYPE:STM_SEND_IMAGE_PROP
	//handling function: Slave::GetAStructForImageInPoint
}STM_SEND_IMAGES_PROP_PARAMS;
typedef struct MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS {
	ImageFromSlave img;
	//MESSAGE_TYPE:MTS_GIVE_THE_CHOSEN_IMAGE
	//handling function:sendRequestToSlaveToGetRealImage
}MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS;
typedef struct STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS {
	RealImage realImg;
	//MESSAGE_TYPE:STM_RESPONSE_TO_THE_CHOSEN_IMAGE
	//handling function:return_of_an_image_by_its_properties
}STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS;
typedef struct MTS_SEND_LOCATION_PARAMS {}MTS_SEND_LOCATION_PARAMS;
typedef struct MTS_MOVE_OUT_OF_THIS_EREA_PARAMS { }MTS_MOVE_OUT_OF_THIS_EREA_PARAMS;
typedef struct MTS_STOP_SEND_LOCATION_PARAMS {}MTS_STOP_SEND_LOCATION_PARAMS;
typedef struct MTS_STOP_AT_PLACE_PARAMS {}MTS_STOP_AT_PLACE_PARAMS;
typedef struct MTS_BACK_TO_STARTING_POINT_PARAMS {}MTS_BACK_TO_STARTING_POINT_PARAMS;
typedef struct MTS_SEND_EVENTS_PARAMS {
	tm& startTime;
	tm& endTime;
	size_t maxSize;
}MTS_SEND_EVENTS_PARAMS;
typedef struct STM_GET_EVENTS_PARAMS {
	vector<Event> events;
}STM_GET_EVENTS_PARAMS;


union MessageParams
{
	M_GOT_REQ_PARAMS M_GOT_REQ_PARAMS;
	MTS_ASK_FOR_IMAGES_PARAMS MTS_ASK_FOR_IMAGES_PARAMS;
	STM_SEND_IMAGES_PROP_PARAMS STM_SEND_IMAGES_PROP_PARAMS;
	MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS;
	STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS;
	MTS_SEND_LOCATION_PARAMS MTS_SEND_LOCATION_PARAMS;
	MTS_MOVE_OUT_OF_THIS_EREA_PARAMS MTS_MOVE_OUT_OF_THIS_EREA_PARAMS;
	MTS_STOP_SEND_LOCATION_PARAMS MTS_STOP_SEND_LOCATION_PARAMS;
	MTS_STOP_AT_PLACE_PARAMS MTS_STOP_AT_PLACE_PARAMS;
	MTS_BACK_TO_STARTING_POINT_PARAMS MTS_BACK_TO_STARTING_POINT_PARAMS;
	MTS_SEND_EVENTS_PARAMS MTS_SEND_EVENTS_PARAMS;
	STM_GET_EVENTS_PARAMS STM_GET_EVENTS_PARAMS;

	// בנאי ברירת מחדל
	MessageParams() {}
	~MessageParams() {}
	//MessageParams(const MessageParams& other)
	//{
	//	//// Logic to determine which member is active in 'other' and copy it
	//	//if (/* condition for M_GOT_REQ_PARAMS being active */) {
	//	//    new (&M_GOT_REQ_PARAMS)struct M_GOT_REQ_PARAMS(other.M_GOT_REQ_PARAMS);
	//	//}
	//	//else if (/* condition for MTS_ASK_FOR_IMAGES_PARAMS being active */) {
	//	//    new (&MTS_ASK_FOR_IMAGES_PARAMS)struct MTS_ASK_FOR_IMAGES_PARAMS(other.MTS_ASK_FOR_IMAGES_PARAMS);
	//	//}
	//	// Add other conditions and member copies as needed
	//}

	// סימון השדה הפעל
	//MessageType activeField;

	

//	// מיישם אופרטור השמה
//	MessageParams& operator=(const MessageParams& other)
//	{
//		if (this != &other) {
//			// לנקות את השדה הנוכחי לפני השמה חדשה
//			destroyCurrentActiveField();
//
//			activeField = other.activeField;
//			switch (activeField) {
//			case MessageType::M_GOT_REQ:
//				new (&M_GOT_REQ_PARAMS)struct M_GOT_REQ_PARAMS(other.M_GOT_REQ_PARAMS);
//				break;
//			case MessageType::MTS_ASK_FOR_IMAGES:
//				new (&MTS_ASK_FOR_IMAGES_PARAMS)struct MTS_ASK_FOR_IMAGES_PARAMS(other.MTS_ASK_FOR_IMAGES_PARAMS);
//				break;
//			case MessageType::STM_SEND_IMAGES_PROP:
//				new (&STM_SEND_IMAGES_PROP_PARAMS)struct STM_SEND_IMAGES_PROP_PARAMS(other.STM_SEND_IMAGES_PROP_PARAMS);
//				break;
//			case MessageType::MTS_GIVE_THE_CHOSEN_IMAGE:
//				new (&MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS)struct MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS(other.MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS);
//				break;
//			case MessageType::STM_RESPONSE_TO_THE_CHOSEN_IMAGE:
//				new (&STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS) struct STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS(other.STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS);
//				break;
//			case MessageType::MTS_SEND_LOCATION:
//				new(&MTS_SEND_LOCATION_PARAMS)struct MTS_SEND_LOCATION_PARAMS(other.MTS_SEND_LOCATION_PARAMS);
//				break;
//			case MessageType::MTS_MOVE_OUT_OF_THIS_EREA:
//				new (&MTS_MOVE_OUT_OF_THIS_EREA_PARAMS) struct MTS_MOVE_OUT_OF_THIS_EREA_PARAMS(other.MTS_MOVE_OUT_OF_THIS_EREA_PARAMS);
//				break;
//			case MessageType::MTS_STOP_SEND_LOCATION:
//				new (&MTS_STOP_SEND_LOCATION_PARAMS)struct MTS_STOP_SEND_LOCATION_PARAMS(other.MTS_STOP_SEND_LOCATION_PARAMS);
//				break;
//			case MessageType::MTS_STOP_AT_PLACE:
//				new (&MTS_STOP_AT_PLACE_PARAMS) struct MTS_STOP_AT_PLACE_PARAMS(other.MTS_STOP_AT_PLACE_PARAMS);
//				break;
//			case MessageType::MTS_BACK_TO_STARTING_POINT:
//				new (&MTS_BACK_TO_STARTING_POINT_PARAMS)struct MTS_BACK_TO_STARTING_POINT_PARAMS(other.MTS_BACK_TO_STARTING_POINT_PARAMS);
//				break;
//			case MessageType::MTS_SEND_EVENTS:
//				new (&MTS_SEND_EVENTS_PARAMS) struct MTS_SEND_EVENTS_PARAMS(other.MTS_SEND_EVENTS_PARAMS);
//				break;
//			case MessageType::STM_GET_EVENTS:
//				new (&STM_GET_EVENTS_PARAMS)struct STM_GET_EVENTS_PARAMS(other.STM_GET_EVENTS_PARAMS);
//				break;
//			default:
//				// לא שדה פעיל
//				activeField = MessageType::size;
//				break;
//			}
//		}
//		return *this;
//	}
//
//	// הרס אופרטור
//	~MessageParams()
//	{
//		destroyCurrentActiveField();
//	}
//
//private:
//	void destroyCurrentActiveField()
//	{
//		switch (activeField) {
//		case 0:
//			M_GOT_REQ_PARAMS.~M_GOT_REQ_PARAMS();
//			break;
//		case 1:
//			MTS_ASK_FOR_IMAGES_PARAMS.~MTS_ASK_FOR_IMAGES_PARAMS();
//			break;
//		case 2:
//			STM_SEND_IMAGES_PROP_PARAMS.~STM_SEND_IMAGES_PROP_PARAMS();
//			break;
//			// הוסף עוד מקרים לכל שדה ב-union
//			// ...
//		default:
//			break;
//		}
//	}
};





