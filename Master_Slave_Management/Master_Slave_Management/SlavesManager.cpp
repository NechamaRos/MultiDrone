#include "SlavesManager.h"
#include "stdio.h"
#include "iostream"
#include "Slave.h"
#include <string>
#include <format>
using namespace std;


#define ARRAYSIZE 5

int GlobalData::static_current_ID = -1;
vector<ImageFromSlave> GlobalData::TheImagesFromSlaves = { {0,0,0,0,0,0} };
int GlobalData::NumOfSlaves_HasToAnswer = ARRAYSIZE;
vector<bool> GlobalData::theBoolAnswers = {};
float GlobalData::MinimumLine = -2;
vector<ImageFromSlave> GlobalData::TheOptimals;
PointForSlave GlobalData::pfs;
mapRequest GlobalData::mapReqGlobal;
vector<RealImage> GlobalData::RealImagesV;
vector<M_GOT_REQ_PARAMS> GlobalData::queueForReqToMap;
bool GlobalData::IsInProcess;
int GlobalData::didntCommunicate = 0;
vector<int> GlobalData::SlavesThatDiDntCommunicate;
STM_SEND_IMAGES_PROP_PARAMS GlobalData::theCurrentAnswer;
STM_RESPONSE_TO_THE_CHOSEN_IMAGE_PARAMS GlobalData::theCurrentRealImageResFromSlave;





void handleMassage1(Message* msg)
{

    Slave slave;
    slave.setId(msg->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId);
    slave.GetAStructForImageInPoint(msg);
}

void handleMassage2()
{
    SlavesManager manager1;
    manager1.getResponse_ImageFromSlave();
}

void handleMessage3(Message* msg)
{
    Slave s;
    s.setId(msg->messageParams->MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS.img.GetSlaveId());
    s.return_of_an_image_by_its_properties(msg);
}

void handleMessage4()
{
    SlavesManager manager2;
    manager2.getTheRealImageFromSlave();
}


void SlavesManager::addEvent(string event)
{
}

bool SlavesManager::handle_photo_request()
{
    for (bool response : GlobalData::theBoolAnswers) {
        if (response == true) {
            return false; // If any slave has a photo, no need to take another
        }
    }
    return true; // No slave has a photo, take a new one
}

bool SlavesManager::verify_photo(bool photo_taken)
{
    return photo_taken;
}

bool SlavesManager::no_comments()
{
    if (GlobalData::NumOfSlaves_HasToAnswer == 5)
        return false;
    return true;
}

bool SlavesManager::forCaseSeveralDidntAnswer(int numOfSlaves)
{
    if (numOfSlaves <= 5)
        return false;
    return true;
}


int SlavesManager::belongs_to_the_current_process(int ID, int current_ID, int number_of_slaves)
{
    if (ID == current_ID) {
        number_of_slaves--;
        return number_of_slaves;
    }

    else
        return number_of_slaves;
}

bool SlavesManager::the_irrelevant_images_did_not_enter_the_array(const vector<int>& IDresponses, int ID)
{
    for (int id : IDresponses) {
        if (id == ID)
            return false;
    }
    return true;
}

bool SlavesManager::request_for_a_map_arrived(bool map_request)
{
    return map_request;
}

bool SlavesManager::Enter_the_queue(int map_request_ID)
{
    for (M_GOT_REQ_PARAMS req : GlobalData::queueForReqToMap)
    {
        if (req.mp.getReqId() == map_request_ID)
            return true;
    }
    return false;
}

bool SlavesManager::The_request_is_processed(int map_request_ID)
{
    if (GlobalData::static_current_ID == map_request_ID)
    {
        cout << "Request number" << map_request_ID << "is being processed";
        return true;
    }
    return false;
}

ImageFromSlave SlavesManager::ChooseTheOptimalImage()
{
    float right = GlobalData::TheImagesFromSlaves[0].GetBR().GetY();
    int image_place = 0;
    for (size_t i = 1; i < GlobalData::TheImagesFromSlaves.size(); i++)
    {
        if (right < GlobalData::TheImagesFromSlaves[i].GetBR().GetY())
        {
            right = GlobalData::TheImagesFromSlaves[i].GetBR().GetY();
            image_place = i;
        }
    }
    GlobalData::TheOptimals.push_back(GlobalData::TheImagesFromSlaves[image_place]);
    return GlobalData::TheImagesFromSlaves[image_place];
};

PointForSlave SlavesManager::next_point_to_send(ImageFromSlave selected_image, float next_minimum_line)
{
    next_minimum_line = selected_image.GetBR().GetX();
    float R;
    if (selected_image.GetBR().GetY() >= GlobalData::mapReqGlobal.GetBR().GetY())
    {
        if (selected_image.GetBR().GetX() <= GlobalData::mapReqGlobal.GetBR().GetX())
        {
            PointForSlave P = PointForSlave(-1, -1);
            return P;
        }
        GlobalData::MinimumLine = next_minimum_line;
        R = GlobalData::mapReqGlobal.GetTL().GetY();
    }
    else
    {
        R = selected_image.GetBR().GetY();
    }
    PointForSlave P = PointForSlave(GlobalData::MinimumLine, R);
    return P;
}

vector<ImageFromSlave> SlavesManager::random_slaves_for_getting_images(ImageFromSlave temporary[])
{
    vector<ImageFromSlave>TheAnswersFromSlaves = { {} };
    for (int i = 0; i < 5; i++)
    {
        Slave s;

        //לקרוא 
        if (s.GetId() > 5)
            s.setId(temporary[i].GetSlaveId());
        vector<ImageFromSlave> ansFromOne = s.CheckIfIHaveImage(GlobalData::pfs, temporary);
        //שליחה למאסטר אם יש או אין ואם יש לשלוח וקטור
        //להשתמש struct של לשלוח הודעה
        for (int j = 0; j < ansFromOne.size(); j++)
        {
            if (TheAnswersFromSlaves[0].GetSlaveId() == 0)
                TheAnswersFromSlaves.clear();
            TheAnswersFromSlaves.push_back(ansFromOne[j]);
        }
    }
    return TheAnswersFromSlaves;
}


void SlavesManager::getARequestForAMap(Message* message)
{
    if (GlobalData::IsInProcess == true)
    {
        //לקרא לטסט שבודק שבאמת נכנס לתור
        GlobalData::queueForReqToMap.push_back(message->messageParams->M_GOT_REQ_PARAMS);
        return;
    }
    GlobalData::IsInProcess = true;
    GlobalData::mapReqGlobal = message->messageParams->M_GOT_REQ_PARAMS.mp;
    GlobalData::static_current_ID = message->messageParams->M_GOT_REQ_PARAMS.mp.getReqId();
    GlobalData::MinimumLine = message->messageParams->M_GOT_REQ_PARAMS.mp.GetTL().GetX();
    PointForSlave pfs1(message->messageParams->M_GOT_REQ_PARAMS.mp.GetTL().GetX(), message->messageParams->M_GOT_REQ_PARAMS.mp.GetTL().GetY());
    GlobalData::pfs = pfs1;
    this->TheWhileFunc();
}


void SlavesManager::getResponse_ImageFromSlave()
{

    //string result = format("handling the response from slave number: {}, in request ID: {}", ResFromSlave.slaveID, ResFromSlave.reqID);
    //this->addEvent(result);
    if (GlobalData::theCurrentAnswer.HaveOrNot_Image)
        if (this->The_request_is_processed(GlobalData::theCurrentAnswer.imgRes.at(0).GetReqId()) == false)
        {
            return;
        }
    if (GlobalData::theCurrentAnswer.communication)
    {
        GlobalData::NumOfSlaves_HasToAnswer--;
        if (GlobalData::theCurrentAnswer.HaveOrNot_Image)
            GlobalData::theBoolAnswers[GlobalData::NumOfSlaves_HasToAnswer] = true;
        for (int i = 0; i < GlobalData::theCurrentAnswer.imgRes.size(); i++)
        {
            GlobalData::TheImagesFromSlaves.push_back(GlobalData::theCurrentAnswer.imgRes[i]);
        }
    }
    else
    {
        GlobalData::didntCommunicate++;
        GlobalData::SlavesThatDiDntCommunicate.push_back(GlobalData::theCurrentAnswer.slaveID);
        //שליחה לטסט לבדיקה של הפונקציה:
       // forCaseSeveralDidntAnswer
        Message* msg = new Message;
        msg->messageParams = new MessageParams;
        msg->messageParams->MTM_SLAVES_NOT_COMMUNICATE_PARAMS.SlavesWithNoCommunication = GlobalData::SlavesThatDiDntCommunicate;
        //אמור להיות פונקציה ששולחת לאופרטור!? את מי שלא ענה
        this->getTheSlavesThatDidntCommunicate(msg);
        //????? GlobalData::IsInProcess = false;

    }
    if (GlobalData::didntCommunicate == ARRAYSIZE)
    {

        //שליחה לטסט לבדיקה של הפונקציה no comments
        //
        string toMsg = "everyone didnt communicate";
        MessageOfNoCommuincationBetwwenAllSlaves msgOfNoCom{ toMsg };

        //אמור להיות פונקציה ששולחת לאופרטור!? את מי שלא ענה
        //צריך פה פונקציה שמקבלת את ההודעה?
        GlobalData::IsInProcess = false;
    }
    //צריך לשאול האם עברה שניה ולא האם זה שווה 0
    if (GlobalData::NumOfSlaves_HasToAnswer == 0)
    {
        bool ThereIsAnImage = false;
        for (size_t i = 0; i < GlobalData::theBoolAnswers.size(); i++)
        {
            if (GlobalData::theBoolAnswers.at(i) == true)
            {
                ThereIsAnImage = true;
                break;
            }
        }
        if (ThereIsAnImage == false)
        {
            Message* msg = new Message;
            msg->messageParams = new MessageParams;

            msg->messageParams->MTS_REQ_FOR_PHOTO_PARAMS.pfs = GlobalData::pfs;
            this->sendMessageToTakeAPicture(msg);
            //איך ממשיכים מפה?
            GlobalData::IsInProcess = false;
            return;
        }
        this->addEvent("Choose the optimal image from the vector of responses");
        ImageFromSlave TheChosenImage = this->ChooseTheOptimalImage();
        this->addEvent("Calculate the next point to send to slaves & quot");
        GlobalData::pfs = this->next_point_to_send(TheChosenImage, TheChosenImage.GetBR().GetX());
        if (GlobalData::pfs.GetX() == -1 && GlobalData::pfs.GetY() == -1)
        {
            for (size_t i = 0; i < GlobalData::TheOptimals.size(); i++)
            {
                // string result = format("Send message to slave {} to get real image by it's properties ", GlobalData::TheOptimals.at(i).GetSlaveId());
                 //this->addEvent(result);
                Message* msg = new Message;
                msg->messageParams = new MessageParams;
                msg->messageParams->MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS.img = GlobalData::TheOptimals.at(i);
                //שליחה לתקשורת
                //SendMessage(mtsR);
                this->sendRequestToSlaveToGetRealImage(msg);
            }
            //שולח בקשות לסלייבים לקבלת תמונות אמיתיות
        }
        else
        {
            this->TheWhileFunc();
        }
    }

}
void SlavesManager::sendRequestToSlaveToGetRealImage(Message* message)
{
    //for (size_t i = 0; i < GlobalData::TheOptimals.size(); i++)
    //{
    //   // string result = format("Send message to slave {} to get real image by it's properties ", GlobalData::TheOptimals.at(i).GetSlaveId());
    //    //this->addEvent(result);
    //    messageToSlaveToGetRealImage mtsR{ GlobalData::TheOptimals.at(i) };
    //    //שליחה לתקשורת
    //    //SendMessage(mtsR);

    //    ResponseToGetRealImageFromSlave res = handleMessage3(mtsR);
    //    handleMessage4(res);
    //}

    handleMessage3(message);
    handleMessage4();
}


void SlavesManager::getTheRealImageFromSlave()
{
    //string result = std::format("handling the response of real image from slave {} ", resFromSlave.realImg.getSlaveId());
    //this->addEvent(result);
    if (GlobalData::theCurrentRealImageResFromSlave.realImg.getSlaveId() != -1)
        GlobalData::RealImagesV.push_back(GlobalData::theCurrentRealImageResFromSlave.realImg);
    if (GlobalData::RealImagesV.size() == GlobalData::TheOptimals.size())
    {
        this->addEvent("Send the parts of the map to openCV API");
        //שליחה לפונקציה של OpenCV
        //שמקבלת וקטור של תמונות אמיתיות ומחזירה את המפה השלמה בתצורה של וקטור על וקטור
        //שליחת המפה השלמה לאופרטור
        GlobalData::RealImagesV.clear();
        GlobalData::TheOptimals.clear();
        GlobalData::IsInProcess = false;
        if (!GlobalData::queueForReqToMap.empty())
        {
            Message* mesg = new Message;
            mesg->messageParams = new MessageParams;
            mesg->messageParams->M_GOT_REQ_PARAMS = GlobalData::queueForReqToMap.at(0);
            this->getARequestForAMap(mesg);
            GlobalData::queueForReqToMap.erase(GlobalData::queueForReqToMap.begin());
        }
    }
}

bool SlavesManager::checking_if_there_is_a_real_picture(ResponseToGetRealImageFromSlave RealI)
{
    if (RealI.realImg.getSlaveId() == -1)
        return false;
    return true;
}

void SlavesManager::sendMessageToTakeAPicture(Message* message)
{
    for (size_t i = 0; i < 5; i++)
    {
        MTS_REQ_FOR_PHOTO_PARAMS rfp{ GlobalData::pfs };
    }
}



void SlavesManager::getTheSlavesThatDidntCommunicate(Message* message)
{
    //get the slaves that didnt communicate and send to the api that take care of it.
};

void SlavesManager::TheWhileFunc()
{
    GlobalData::NumOfSlaves_HasToAnswer = ARRAYSIZE;
    GlobalData::theBoolAnswers = { false, false, false, false, false };
    GlobalData::TheImagesFromSlaves.clear();
    GlobalData::SlavesThatDiDntCommunicate.clear();
    GlobalData::didntCommunicate = 0;
    Slave mySlaves[ARRAYSIZE];
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        if (mySlaves[i].GetId() > 5)
            mySlaves[i].setId(i);
        // string result = format("message to slave {} to get image properties in specific point ", mySlaves[i].GetId());
        // this->addEvent(result);

        Message* mesg = new Message;
        mesg->messageParams = new MessageParams;
        mesg->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs = GlobalData::pfs;
        mesg->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId = GlobalData::static_current_ID;
        mesg->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId = i;



        //שליחה לתקשורת
        //SendMessage(msg);

        handleMassage1(mesg);
        handleMassage2();
    }
};




