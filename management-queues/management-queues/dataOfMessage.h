#pragma once
#include "MessageType.h"
#include "Priority.h"
#include "Message.h"

struct dataOfMessage {
    Priority priority;
    void (*func)(Message* message);
};

//functions
void getARequestForAMap(Message* message) {};//M_GOT_REQ
void sendRequestForImageInSpecificPoint(Message* message) {};//MTS_ASK_FOR_IMAGES
void GetAStructForImageInPoint(Message* message) {};//STM_SEND_IMAGE_PROP
void sendRequestToSlaveToGetRealImage(Message* message) {};//MTS_GIVE_THE_CHOSEN_IMAGE
void return_of_an_image_by_its_properties(Message* message) {};//STM_RESPONSE_TO_THE_CHOSEN_IMAGE
void sendMessageToTakeAPicture(Message* message) {};//MTS_REQ_FOR_PHOTO
void getTheSlavesThatDidntCommunicate(Message* message) {};//MTM_SLAVES_NOT_COMMUNICATE

void receiveEventsFromSlave(Message* message) {};//STM_GET_EVENTS_PARAMS
void getEvents(Message* message) {};//MTS_SEND_EVENTS_PARAMS

void handleLocation(Message* message) {};//MTS_SEND_LOCATIONint sec
void moveFromYourPlaceSafely(Message* message) {};//MTS_MOVE_OUT_OF_THIS_AREA
void setSendLocation(Message* message) {};//MTS_STOP_SEND_LOCATION
void updateDroneLocation(Message* message) {};//STM_LOCATION
void getTheSlavesThatDidntCommunicate(Message* message);//MTM_SLAVES_NOT_COMMUNICATE


dataOfMessage dataOfMessageVector[int(MessageType::size)];

//initial vector pointer to function
void InitialPointerToFunction() {
    dataOfMessageVector[static_cast<size_t>(MessageType::M_GOT_REQ)].func = &getARequestForAMap;
    dataOfMessageVector[static_cast<size_t>(MessageType::M_GOT_REQ)].priority = Priority::Medium;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_ASK_FOR_IMAGES)].func = &sendRequestForImageInSpecificPoint;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_ASK_FOR_IMAGES)].priority = Priority::MediumHigh;
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_SEND_IMAGES_PROP)].func = &GetAStructForImageInPoint;
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_SEND_IMAGES_PROP)].priority = Priority::Medium;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_GIVE_THE_CHOSEN_IMAGE)].func = &sendRequestToSlaveToGetRealImage;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_GIVE_THE_CHOSEN_IMAGE)].priority = Priority::MediumHigh;
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_RESPONSE_TO_THE_CHOSEN_IMAGE)].func = &return_of_an_image_by_its_properties;
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_RESPONSE_TO_THE_CHOSEN_IMAGE)].priority = Priority::High;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_REQ_FOR_PHOTO)].func = &sendMessageToTakeAPicture;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_REQ_FOR_PHOTO)].priority = Priority::MediumLow;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTIP_REQ_FOR_REAL_MAP)].func = &getEvents;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTIP_REQ_FOR_REAL_MAP)].priority = Priority::Medium;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_SEND_LOCATION)].func = &handleLocation;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_SEND_LOCATION)].priority = Priority::MediumLow;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_MOVE_OUT_OF_THIS_EREA)].func = &moveFromYourPlaceSafely;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_MOVE_OUT_OF_THIS_EREA)].priority = Priority::Medium;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_STOP_SEND_LOCATION)].func = &setSendLocation;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_STOP_SEND_LOCATION)].priority = Priority::Low;
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_LOCATION)].func = updateDroneLocation;
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_LOCATION)].priority = Priority::Low;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_STOP_AT_PLACE)].func = nullptr;//no function
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_STOP_AT_PLACE)].priority = Priority::Medium;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_BACK_TO_STARTING_POINT)].func = nullptr;//no function
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_BACK_TO_STARTING_POINT)].priority = Priority::High;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_SEND_EVENTS)].func = &getEvents;
    dataOfMessageVector[static_cast<size_t>(MessageType::MTS_SEND_EVENTS)].priority = Priority::MediumLow;
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_GET_EVENTS)].func = &receiveEventsFromSlave;
    dataOfMessageVector[static_cast<size_t>(MessageType::STM_GET_EVENTS)].priority = Priority::MediumHigh;
}


