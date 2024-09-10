#include <vector>
#include "ImageFromSlave.h"
#include "mapRequest.h"
#include "Structs.h"
#include "Message.h"

using namespace std;


struct GlobalData {
    static int static_current_ID;
    static vector<ImageFromSlave> TheImagesFromSlaves;
    static STM_SEND_IMAGES_PROP_PARAMS theCurrentAnswer;
    static STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS theCurrentRealImageResFromSlave;
    static int NumOfSlaves_HasToAnswer;
    static vector<bool> theBoolAnswers;
    static float MinimumLine;
    static vector<ImageFromSlave> TheOptimals;
    static PointForSlave pfs;
    static mapRequest mapReqGlobal;
    static vector<RealImage> RealImagesV;
    static bool IsInProcess;
    static vector<M_GOT_REQ_PARAMS> queueForReqToMap;
    static int didntCommunicate;
    static vector<int>SlavesThatDiDntCommunicate;




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
    bool Enter_the_queue(int map_request_ID);
    bool The_request_is_processed(int map_request_ID);

    ImageFromSlave ChooseTheOptimalImage();

    PointForSlave next_point_to_send(ImageFromSlave selected_image, float next_minimum_line);

    vector<ImageFromSlave>random_slaves_for_getting_images(ImageFromSlave temporary[]);
    void getARequestForAMap(Message* message);


    void getResponse_ImageFromSlave();
    void sendRequestToSlaveToGetRealImage(Message* message);//MTS_GIVE_THE_CHOSEN_IMAGE

    void getTheRealImageFromSlave();

    bool checking_if_there_is_a_real_picture(ResponseToGetRealImageFromSlave RealI);
    void sendMessageToTakeAPicture(Message* message);
    bool forCaseSeveralDidntAnswer(int numOfSlaves);

    void getTheSlavesThatDidntCommunicate(Message* message);
    void TheWhileFunc();
};



//void getARequestForAMap(Message* message) {};//M_GOT_REQ
//void sendRequestForImageInSpecificPoint(Message* message) {};//MTS_ASK_FOR_IMAGES
//void GetAStructForImageInPoint(Message* message) {};//STM_SEND_IMAGE_PROP
//void sendRequestToSlaveToGetRealImage(Message* message) {};//MTS_GIVE_THE_CHOSEN_IMAGE
//void return_of_an_image_by_its_properties(Message* message) {};//STM_RESPONSE_TO_THE_CHOSEN_IMAGE
//void sendMessageToTakeAPicture(Message* message) {};//MTS_REQ_FOR_PHOTO
//void getTheSlavesThatDidntCommunicate(Message* message) {};//MTM_SLAVES_NOT_COMMUNICATE
