#pragma once
#include <vector>
#include "ImageFromSlave.h"
#include "mapRequest.h"
#include "MessageParams.h"
#include "RealImage.h"
using namespace std;
struct GlobalData {
    static int static_current_ID;
    static vector<ImageFromSlave> TheImagesFromSlaves;
    static int NumOfSlaves_HasToAnswer;
    static vector<bool> theBoolAnswers;
    static float MinimumLine;
    static vector<ImageFromSlave> TheOptimals;
    static PointForSlave pfs;
    static mapRequest mapReqGlobal;
    static vector<RealImage> RealImagesV;
};
class SlavesManager {
public:
    void addEvent(string event);
    bool handle_photo_request();
    bool verify_photo(bool photo_taken);
    bool no_comments();
    int belongs_to_the_current_process(int ID, int current_ID, int number_of_slaves);
    bool the_irrelevant_images_did_not_enter_the_array(const vector<int>& IDresponses, int current_ID);
    bool request_for_a_map_arrived(bool map_request);
    bool Enter_the_queue(vector<mapRequest> queue, int map_request_ID);
    bool The_request_is_processed(int map_request_ID);
    ImageFromSlave ChooseTheOptimalImage();
    PointForSlave next_point_to_send(ImageFromSlave selected_image, float next_minimum_line);
    vector<ImageFromSlave>random_slaves_for_getting_images(ImageFromSlave temporary[]);
    PointForSlave getARequestForAMap(M_GOT_REQ_PARAMS getReqMap);
    vector<STM_SEND_IMAGES_PROP_PARAMS> sendRequestForImageInSpecificPoint();
    void getResponse_ImageFromSlave(STM_SEND_IMAGES_PROP_PARAMS ResFromSlave);
    void sendRequestToSlaveToGetRealImage();
    void getTheRealImageFromSlave(STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS resFromSlave);
    bool checking_if_there_is_a_real_picture(STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS RealI);
    void sendMessageToTakeAPicture();
    bool forCaseSeveralDidntAnswer(int numOfSlaves);
    void getTheSlavesThatDidntCommunicate(MTM_SLAVES_NOT_COMMUNICATE_PARAMS msgToMaster);
    void TheWhileFunc();
};