#pragma once
#include "ImageFromSlave.h"

#include <vector>
#include "mapRequest.h"
#include "RealImage.h"

using namespace std;

enum MessageTypesForReq
{
    M_GOT_REQ,
    MTS_ASK_FOR_IMAGES,
    STM_SEND_IMAGE_PROP,
    MTS_GIVE_THE_CHOSEN_IMAGE,
    STM_RESPONSE_TO_THE_CHOSEN_IMAGE,
    MTIP_REQ_FOR_REAL_MAP,
    MTS_REQ_FOR_PHOTO,
    MTM_NO_COMMUNICATION_AT_ALL,
    MTM_SLAVES_NOT_COMMUNICATE,
    //IPTM_SEND_THE_IMAGES,
};

struct getRequestForMap
{
    mapRequest mp;
    //MESSAGE_TYPE:M_GOT_REQ
    //handling function: SlavesManager::getARequestForAMap
};
struct messageToSlaveToGetImage
{
    PointForSlave pfs;
    int slaveId;
    int reqId;

    //MESSAGE_TYPE:MTS_ASK_FOR_IMAGES
    //handling function: Slave::GetAStructForImageInPoint
};
struct ResponseToGetImageFromSlave
{
    int reqID;
    int slaveID;
    bool HaveOrNot_Image;
    bool communication = false;
    vector<ImageFromSlave> imgRes = {};
    //MESSAGE_TYPE:STM_SEND_IMAGE_PROP
    //handling function: SlaveManager::getResponse_ImageFromSlave
};


struct messageToSlaveToGetRealImage
{
    ImageFromSlave img;
    //MESSAGE_TYPE:MTS_GIVE_THE_CHOSEN_IMAGE
    //handling function:Slave::return_of_an_image_by_its_properties
};

struct ResponseToGetRealImageFromSlave
{
    RealImage realImg;
    //MESSAGE_TYPE:STM_RESPONSE_TO_THE_CHOSEN_IMAGE
    //handling function:SlaveManager::getTheRealImageFromSlave
};

struct RequestToReceiveAMapFromOpenCV
{
    vector<RealImage> realImages;
    //MESSAGE_TYPE: MTIP_REQ_FOR_REAL_MAP,
    //handling function:openCV API has to get the message and handle it.
};

struct CallARequestForAPhoto
{
    PointForSlave pfs;
    //MESSAGE_TYPE:MTS_REQ_FOR_PHOTO,
    //handling function:SlavesManager::sendMessageToTakeAPicture(PointForSlave pfs)
};

struct MessageOfNoCommuincationBetwwenAllSlaves
{
    string msg22;
    //MESSAGE_TYPE:MTM_NO_COMMUNICATION_AT_ALL,
    //handling function:__________________
};
struct GetTheSlavesThatDidntCommunicate
{
    vector<int> SlavesWithNoCommunication;
    //MESSAGE_TYPE:MTM_SLAVES_NOT_COMMUNICATE
    //handling function:SlavesManager::getTheSlavesThatDidntCommunicate
};


//void getARequestForAMap(Message* message) {};//M_GOT_REQ
//void sendRequestForImageInSpecificPoint(Message* message) {};//MTS_ASK_FOR_IMAGES
//void GetAStructForImageInPoint(Message* message) {};//STM_SEND_IMAGE_PROP
//void sendRequestToSlaveToGetRealImage(Message* message) {};//MTS_GIVE_THE_CHOSEN_IMAGE
//void return_of_an_image_by_its_properties(Message* message) {};//STM_RESPONSE_TO_THE_CHOSEN_IMAGE
//void sendMessageToTakeAPicture(Message* message) {};//MTS_REQ_FOR_PHOTO
//void getTheSlavesThatDidntCommunicate(Message* message) {};//MTM_SLAVES_NOT_COMMUNICATE
//void receiveEventsFromSlave(Message* message) {};//STM_GET_EVENTS_PARAMS
//void getEvents(Message* message) {};//MTS_SEND_EVENTS_PARAMS
//void handleLocation(Message* message) {};//MTS_SEND_LOCATIONint sec
//void moveFromYourPlaceSafely(Message* message) {};//MTS_MOVE_OUT_OF_THIS_AREA
//void setSendLocation(Message* message) {};//MTS_STOP_SEND_LOCATION
//void updateDroneLocation(Message* message) {};//STM_LOCATION
//







