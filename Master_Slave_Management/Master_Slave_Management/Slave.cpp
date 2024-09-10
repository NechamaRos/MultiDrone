#include "Slave.h"
#include "SlavesManager.h"
#include <ctime>

int Slave::Counter = 0;
using namespace std;

Mat m1 = imread("C:\\Users\\User\\Desktop\\פרקטיקיץ\\images\\צילום מסך 2024 - 07 - 25 121434.png");
RealImage r1 = { 0, 0, m1 };
RealImage r2 = { 1, 1, m1 };
RealImage r3 = { 2, 2, m1 };
RealImage r4 = { 3, 3, m1 };
RealImage r5 = { 4, 4, m1 };
RealImage r6 = { 5, 5, m1 };
RealImage r7 = { 6, 6, m1 };
RealImage r8 = { 7, 7, m1 };
RealImage r9 = { 8, 8, m1 };
RealImage r10 = { 9, 9, m1 };
RealImage r11 = { 10, 10, m1 };
RealImage r12 = { 11, 0, m1 };
RealImage r13 = { 12, 1, m1 };
RealImage r14 = { 13, 2, m1 };
RealImage r15 = { 14, 3, m1 };
RealImage r16 = { 15, 4, m1 };
RealImage r30 = { 30, 4, m1 };
RealImage r36 = { 36, 4, m1 };
RealImage r45 = { 45, 4, m1 };
vector<RealImage> imgs = { r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16,r30,r36,r45 };
ImageFromSlave temporary0[6] =
{
	{  31.8200,35.1766,31.8000,35.2075,1,0},
	{ 31.8000,35.1766,31.7961,35.2075,2,0},
	//{ 31.8200, 35.1766, 31.7961, 35.2075,1,12 },
	{ 31.8200, 35.1766, 31.6060, 35.1930,2,0 },
	{ 31.8140, 35.1860, 31.8000, 35.2025,3,0 },
	{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,4,0 },
	{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,5,0 }
};
ImageFromSlave temporary1[6] =
{
	{  31.8200,35.1766,31.8000,35.2075,1,1},
	{ 31.8000,35.1766,31.7961,35.2075,2,1},
	//{ 31.8200, 35.1766, 31.7961, 35.2075,1,12 },
	{ 31.8200, 35.1766, 31.6060, 35.1930,2,1 },
	{ 31.8140, 35.1860, 31.8000, 35.2025,3,1 },
	{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,4,1 },
	{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,5,1 }
};
ImageFromSlave temporary2[6] =
{
	{  31.8200,35.1766,31.8000,35.2075,1,2},
	{ 31.8000,35.1766,31.7961,35.2075,2,2},
	//{ 31.8200, 35.1766, 31.7961, 35.2075,1,12 },
	{ 31.8200, 35.1766, 31.6060, 35.1930,2,2 },
	{ 31.8140, 35.1860, 31.8000, 35.2025,3,2 },
	{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,4,2 },
	{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,5,2 }
};

ImageFromSlave temporary3[6] =
{
	{  100,100,100,100,1,3},
	{ 100,100,100,100,2,3},
	//{ 31.8200, 35.1766, 31.7961, 35.2075,1,12 },
	{ 100,100,100,100,2,3 },
	{ 100,100,100,100,3,3 },
	{ 100,100,100,100,4,3 },
	{ 100,100,100,100,5,3 }
};

ImageFromSlave temporary6[6] = {
	//31.8200, 35.1766);
		//31.7961, 35.2075);
	{ 31.8200, 35.1766,31.7961, 35.2075,1,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,1,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,2,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,3,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,4,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,5,6},
};
ImageFromSlave temporary7[6] = {
	//31.8200, 35.1766);
		//31.7961, 35.2075);
	{ 31.8200, 35.1766,31.7961, 35.2075,0,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,1,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,2,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,3,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,4,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,5,7},
};

ImageFromSlave temporary8[6] = {
	//31.8200, 35.1766);
		//31.7961, 35.2075);
	{ 31.8200, 35.1766,31.7961, 35.2075,0,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,1,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,2,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,0,8},
	{ 31.8200, 35.1766,31.7961, 35.2075,1,8},
	{ 31.8200, 35.1766,31.7961, 35.2075,2,8},
};
ImageFromSlave temporaryforme[5] =
{
	{ 32.7897f, 35.5785f, 32.7915f, 35.5809f,1,8 },
	{ 32.7880f, 35.5733f, 32.7902f, 35.5757f,1,8 },
	{ 32.7925f, 35.5769f, 32.7948f, 35.5792f,3,8 },
	{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,3,8 },
	{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,5,8 }
};
vector<ImageFromSlave>  Slave::CheckIfIHaveImage(PointForSlave p, ImageFromSlave MyArrOfImages[])
{
	vector<ImageFromSlave> ans;
	for (int i = 0; i < 6; i++)
	{
		if (MyArrOfImages[i].GetSlaveId() == GlobalData::theCurrentAnswer.slaveID && MyArrOfImages[i].GetReqId() == GlobalData::static_current_ID)
		{
			if (isPointInImage(p, MyArrOfImages[i]))
			{
				ans.push_back(MyArrOfImages[i]);
			}
		}
	}
	return ans;
}

bool Slave::isPointInImage(PointForSlave p, ImageFromSlave img)
{
	//&& p.GetX() <= img.GetBR().GetX() 
	//&& p.GetY() <= img.GetBR().GetY()
	if (p.GetX() >= img.GetTL().GetX() &&
		p.GetY() <= img.GetTL().GetY())
		return true;
	else {
		return false;
	}
}
//void GetAStructForImageInPoint(Message* message) {};//STM_SEND_IMAGE_PROP

void Slave::GetAStructForImageInPoint(Message* message)
{
	SlavesManager manager;
	manager.addEvent("slave handle the request to get image in specific point");
	//ResponseToGetImageFromSlave res;
	GlobalData::theCurrentAnswer.HaveOrNot_Image = NULL;
	vector<ImageFromSlave> ans;
	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 0)
	{
		ans = CheckIfIHaveImage(message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs, temporary0);
		GlobalData::theCurrentAnswer.communication = true;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}
	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 1)
	{
		ans = CheckIfIHaveImage(message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs, temporary1);
		GlobalData::theCurrentAnswer.communication = true;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}
	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 2)
	{
		ans = CheckIfIHaveImage(message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs, temporary2);
		GlobalData::theCurrentAnswer.communication = true;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}
	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 3)
	{
		ans = CheckIfIHaveImage(message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs, temporary3);
		GlobalData::theCurrentAnswer.communication = true;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}

	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 4)
	{
		//ans = CheckIfIHaveImage(msg.pfs, temporary5);
		GlobalData::theCurrentAnswer.communication = false;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}
	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 6)
	{
		ans = CheckIfIHaveImage(message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs, temporary6);
		srand(time(0));
		// יצירת ערך אקראי בין true ל-false
		//bool randomBool = std::rand() % 2;
		if (temporary6->GetSlaveId() % 2 == 1)
			GlobalData::theCurrentAnswer.communication = true;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}
	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 7)
	{
		ans = CheckIfIHaveImage(message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs, temporary7);
		GlobalData::theCurrentAnswer.communication = true;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}
	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 8)
	{
		ans = CheckIfIHaveImage(message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs, temporary8);
		GlobalData::theCurrentAnswer.communication = true;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}
	if (message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId == 56)
	{
		ans = CheckIfIHaveImage(message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.pfs, temporaryforme);
		GlobalData::theCurrentAnswer.communication = true;
		GlobalData::theCurrentAnswer.slaveID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.slaveId;
		GlobalData::theCurrentAnswer.reqID = message->messageParams->MTS_ASK_FOR_IMAGES_PROP_PARAMS.reqId;
	}
	if (ans.empty())
		GlobalData::theCurrentAnswer.HaveOrNot_Image = false;
	else
		GlobalData::theCurrentAnswer.HaveOrNot_Image = true;
	GlobalData::theCurrentAnswer.imgRes = ans;
}

void Slave::return_of_an_image_by_its_properties(Message* message)
{
	SlavesManager manager;
	string result = format("slave {} handling the request to get real image ", message->messageParams->MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS.img.GetSlaveId());
	manager.addEvent(result);
	for (RealImage img : imgs)
	{
		if (message->messageParams->MTS_GIVE_THE_CHOSEN_IMAGE_PARAMS.img.GetId() == img.getId())
		{
			GlobalData::theCurrentRealImageResFromSlave.realImg = img;
			return;
		}
	}
	RealImage img1;
	GlobalData::theCurrentRealImageResFromSlave.realImg = img1;
}

//פונקציה במסטר לא קראת לפונקציה בסלייב.
//יש לשים לב שכל מה שצריך לקראת גם באמיתי נמצא בקוד ולא בקובץ טסט. 



