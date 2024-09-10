
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN 

#include "docTest.h"
#include <iostream>
#include <cstdlib>  
#include <ctime> 
#include <random>
#include "ImageFromSlave.h"
#include "Slave.h"
#include "mapRequest.h"
#include "SlavesManager.h"

using namespace std;

ImageFromSlave Temporary0[6] =
{
	{  31.8200,35.1766,31.8000,35.2075,1,0},
	{ 31.8000,35.1766,31.7961,35.2075,2,0},
	//{ 31.8200, 35.1766, 31.7961, 35.2075,1,12 },
	{ 31.8200, 35.1766, 31.6060, 35.1930,2,0 },
	{ 31.8140, 35.1860, 31.8000, 35.2025,3,0 },
	{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,4,0 },
	{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,5,0 }
};
ImageFromSlave Temporary1[6] =
{
	{  31.8200,35.1766,31.8000,35.2075,1,1},
	{ 31.8000,35.1766,31.7961,35.2075,2,1},
	//{ 31.8200, 35.1766, 31.7961, 35.2075,1,12 },
	{ 31.8200, 35.1766, 31.6060, 35.1930,2,1 },
	{ 31.8140, 35.1860, 31.8000, 35.2025,3,1 },
	{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,4,1 },
	{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,5,1 }
};
ImageFromSlave Temporary2[6] =
{
	{  31.8200,35.1766,31.8000,35.2075,1,2},
	{ 31.8000,35.1766,31.7961,35.2075,2,2},
	//{ 31.8200, 35.1766, 31.7961, 35.2075,1,12 },
	{ 31.8200, 35.1766, 31.6060, 35.1930,2,2 },
	{ 31.8140, 35.1860, 31.8000, 35.2025,3,2 },
	{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,4,2 },
	{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,5,2 }
};

ImageFromSlave Temporary3[6] =
{
	{  100,100,100,100,1,3},
	{ 100,100,100,100,2,3},
	//{ 31.8200, 35.1766, 31.7961, 35.2075,1,12 },
	{ 100,100,100,100,2,3 },
	{ 100,100,100,100,3,3 },
	{ 100,100,100,100,4,3 },
	{ 100,100,100,100,5,3 }
};

ImageFromSlave Temporary6[6] = {
	//31.8200, 35.1766);
		//31.7961, 35.2075);
	{ 31.8200, 35.1766,31.7961, 35.2075,1,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,1,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,2,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,3,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,4,6},
	{ 31.8200, 35.1766,31.7961, 35.2075,5,6},
};
ImageFromSlave Temporary7[6] = {
	//31.8200, 35.1766);
		//31.7961, 35.2075);
	{ 31.8200, 35.1766,31.7961, 35.2075,0,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,1,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,2,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,3,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,4,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,5,7},
};

ImageFromSlave Temporary8[6] = {
	//31.8200, 35.1766);
		//31.7961, 35.2075);
	{ 31.8200, 35.1766,31.7961, 35.2075,0,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,1,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,2,7},
	{ 31.8200, 35.1766,31.7961, 35.2075,0,8},
	{ 31.8200, 35.1766,31.7961, 35.2075,1,8},
	{ 31.8200, 35.1766,31.7961, 35.2075,2,8},
};

//addEvent(".........")
//הכל יתרחש בתוך פחות משניה.
TEST_CASE("test end to end")
{
	SlavesManager manager;
	mapRequest MapReq;
	Message* message = new Message;
	message->messageParams = new MessageParams;
	message->messageParams->M_GOT_REQ_PARAMS.mp = MapReq;
	manager.addEvent("get request for a map");
	manager.getARequestForAMap(message);

	CHECK(manager.The_request_is_processed(GlobalData::mapReqGlobal.getReqId()) == true);

	//check that at least one answered yet
	//CHECK(manager.handle_photo_request() == false);

	//check that everyone answered
	//CHECK(manager.no_comments() == true);

	//CHECK(GlobalData::TheOptimals[0].GetBR().GetY() == Temporary0[0].GetBR().GetY());
	//CHECK(GlobalData::TheOptimals[0].GetBR().GetX() == Temporary0[0].GetBR().GetX());
	//CHECK(GlobalData::TheOptimals[0].GetTL().GetY() == Temporary0[0].GetTL().GetY());
	//CHECK(GlobalData::TheOptimals[0].GetTL().GetX() == Temporary0[0].GetTL().GetX());
	//						   
	//CHECK(GlobalData::TheOptimals[1].GetBR().GetY() == Temporary0[1].GetBR().GetY());
	//CHECK(GlobalData::TheOptimals[1].GetBR().GetX() == Temporary0[1].GetBR().GetX());
	//CHECK(GlobalData::TheOptimals[1].GetTL().GetY() == Temporary0[1].GetTL().GetY());
	//CHECK(GlobalData::TheOptimals[1].GetTL().GetX() == Temporary0[1].GetTL().GetX());
}


//Several requests to map at the same time
TEST_CASE("test end to end  - case several requests to map at the same time")
{
	SlavesManager manager;
	mapRequest MapReq;
	Message* message = new Message;
	message->messageParams = new MessageParams;
	message->messageParams->M_GOT_REQ_PARAMS.mp = MapReq;
	manager.addEvent("get request for a map");
	manager.getARequestForAMap(message);



	Message* message1 = new Message;
	message->messageParams->M_GOT_REQ_PARAMS.mp = MapReq;
	manager.addEvent("get request for a map");
	manager.getARequestForAMap(message1);


	CHECK(manager.The_request_is_processed(GlobalData::mapReqGlobal.getReqId()) == true);

	//CHECK(manager.Enter_the_queue(getreqMap1.mp.getReqId()) == true);


	//CHECK(GlobalData::TheOptimals[0].GetBR().GetY() == Temporary1[0].GetBR().GetY());
	//CHECK(GlobalData::TheOptimals[0].GetBR().GetX() == Temporary1[0].GetBR().GetX());
	//CHECK(GlobalData::TheOptimals[0].GetTL().GetY() == Temporary1[0].GetTL().GetY());
	//CHECK(GlobalData::TheOptimals[0].GetTL().GetX() == Temporary1[0].GetTL().GetX());
	//								
	//CHECK(GlobalData::TheOptimals[1].GetBR().GetY() == Temporary1[1].GetBR().GetY());
	//CHECK(GlobalData::TheOptimals[1].GetBR().GetX() == Temporary1[1].GetBR().GetX());
	//CHECK(GlobalData::TheOptimals[1].GetTL().GetY() == Temporary1[1].GetTL().GetY());
	//CHECK(GlobalData::TheOptimals[1].GetTL().GetX() == Temporary1[1].GetTL().GetX());

	//CHECK(manager.The_request_is_processed(GlobalData::mapReqGlobal.getReqId()) == true);


	//CHECK(GlobalData::TheOptimals[0].GetBR().GetY() == Temporary2[0].GetBR().GetY());
	//CHECK(GlobalData::TheOptimals[0].GetBR().GetX() == Temporary2[0].GetBR().GetX());
	//CHECK(GlobalData::TheOptimals[0].GetTL().GetY() == Temporary2[0].GetTL().GetY());
	//CHECK(GlobalData::TheOptimals[0].GetTL().GetX() == Temporary2[0].GetTL().GetX());
	//														
	//CHECK(GlobalData::TheOptimals[1].GetBR().GetY() == Temporary2[1].GetBR().GetY());
	//CHECK(GlobalData::TheOptimals[1].GetBR().GetX() == Temporary2[1].GetBR().GetX());
	//CHECK(GlobalData::TheOptimals[1].GetTL().GetY() == Temporary2[1].GetTL().GetY());
	//CHECK(GlobalData::TheOptimals[1].GetTL().GetX() == Temporary2[1].GetTL().GetX());

}


////Everyone sends a negative answer

TEST_CASE("test end to end - case everyone sends a negative answer")
{
	SlavesManager manager;
	mapRequest MapReq;
	Message* message = new Message;
	message->messageParams = new MessageParams;
	message->messageParams->M_GOT_REQ_PARAMS.mp = MapReq;
	manager.addEvent("get request for a map");
	manager.getARequestForAMap(message);


	CHECK(manager.The_request_is_processed(GlobalData::mapReqGlobal.getReqId()) == true);

	////check that everyone answered
	//CHECK(manager.no_comments(NumOfSlaves_HasToAnswer) == true);

	////check that at least one answered yet
	//CHECK(manager.handle_photo_request(theBoolAnswers) == true);

}


//Several photo requests that are not answered

TEST_CASE("test end to end - case everyone didn't answered")
{
	SlavesManager manager;
	mapRequest MapReq;
	Message* message = new Message;
	message->messageParams = new MessageParams;
	message->messageParams->M_GOT_REQ_PARAMS.mp = MapReq;
	manager.addEvent("get request for a map");
	manager.getARequestForAMap(message);

	CHECK(manager.The_request_is_processed(GlobalData::mapReqGlobal.getReqId()) == true);

	//check that everyone answered
	//CHECK(manager.no_comments(NumOfSlaves_HasToAnswer) == false);

}


//case several didnt communicate
//TEST_CASE("test end to end - case several requests not answered")
//{
//	SlavesManager manager;
//	mapRequest MapReq;
//	Message* message = new Message;
//	message->messageParams = new MessageParams;
//	message->messageParams->M_GOT_REQ_PARAMS.mp = MapReq;
//	manager.addEvent("get request for a map");
//	manager.getARequestForAMap(message);
//
//	CHECK(manager.The_request_is_processed(GlobalData::mapReqGlobal.getReqId()) == true);
//
//	//CHECK(manager.forCaseSeveralDidntAnswer(NumOfSlaves_HasToAnswer) == false);
//
//	//CHECK(manager.no_comments(NumOfSlaves_HasToAnswer) == true);
//
//	//CHECK(manager.handle_photo_request(theBoolAnswers) == false);
//		
//	//CHECK(TheOptimals[0].GetBR().GetY() == Temporary6[0].GetBR().GetY());
//	//CHECK(TheOptimals[0].GetBR().GetX() == Temporary6[0].GetBR().GetX());
//	//CHECK(TheOptimals[0].GetTL().GetY() == Temporary6[0].GetTL().GetY());
//	//CHECK(TheOptimals[0].GetTL().GetX() == Temporary6[0].GetTL().GetX());
//}


////Test response handling after timer
//
//TEST_CASE("test end to end - case response handling after timer")
//{
//	vector<RealImage> realImages;
//	vector<ImageFromSlave> TheOptimals;
//	mapRequest MapReq;
//	SlavesManager manager;
//	float MinimumLine;
//	getRequestForMap getreqMap = { MapReq };
//	PointForSlave pfs = manager.getARequestForAMap(getreqMap, MinimumLine);
//
//	CHECK(manager.The_request_is_processed(MapReq.getReqId()) == true);
//
//#define INVALIDNUM -1
//
//	while (pfs.GetX() != INVALIDNUM && pfs.GetY() != INVALIDNUM)//כל עוד זה לא סוף התמונה
//	{
//		vector<ImageFromSlave> TheImagesFromSlaves = { {0,0,0,0,0,0} };
//		vector<ResponseToGetImageFromSlave> resultFromSlaves = {};
//
//#define ARRAYSIZE 5
//		vector<bool> theBoolAnswers(ARRAYSIZE, false);
//
//		int NumOfSlaves_HasToAnswer = ARRAYSIZE;
//
//		Slave mySlaves[ARRAYSIZE];
//		auto startTime = chrono::steady_clock::now();
//		auto endTime = startTime + chrono::seconds(1);
//		while (chrono::steady_clock::now() < endTime)
//		{
//			for (int i = 0; i < ARRAYSIZE; i++)
//			{
//				if (mySlaves[i].GetId() > 5)
//					mySlaves[i].setId(i);
//
//				messageToSlaveToGetImage msg = { pfs,i, MapReq.getReqId() };
//				ResponseToGetImageFromSlave resFromSlaveToTheMaster = mySlaves[i].GetAStructForImageInPoint(msg);
//				if (resFromSlaveToTheMaster.HaveOrNot_Image)
//					CHECK(manager.The_request_is_processed(resFromSlaveToTheMaster.imgRes.at(0).GetReqId()) == true);
//				ResponseToGetImageFromSlave res =
//					manager.getResponse_ImageFromSlave(resFromSlaveToTheMaster, TheImagesFromSlaves,
//						NumOfSlaves_HasToAnswer, theBoolAnswers);
//				auto startTime = chrono::steady_clock::now();
//				auto endTime = startTime + chrono::seconds(1);
//				while (chrono::steady_clock::now() < endTime) {}
//			}
//		}
//		//check that at least one answered yet
//		CHECK(manager.handle_photo_request(theBoolAnswers) == false);
//
//		//check that everyone answered
//		CHECK(manager.no_comments(NumOfSlaves_HasToAnswer) == true);
//
//		ImageFromSlave TheChosenImage = manager.ChooseTheOptimalImage(TheImagesFromSlaves, TheOptimals);
//		pfs = manager.next_point_to_send(TheChosenImage,
//			MapReq, MinimumLine, TheChosenImage.GetBR().GetX());
//	}
//
//	CHECK(TheOptimals[0].GetBR().GetY() == Temporary7[0].GetBR().GetY());
//	CHECK(TheOptimals[0].GetBR().GetX() == Temporary7[0].GetBR().GetX());
//	CHECK(TheOptimals[0].GetTL().GetY() == Temporary7[0].GetTL().GetY());
//	CHECK(TheOptimals[0].GetTL().GetX() == Temporary7[0].GetTL().GetX());
//
//
//	Slave mySlaves[10] = {};
//	for (int i = 0; i < 10; i++)
//	{
//		mySlaves[i].setId(i);
//	}
//	for (size_t i = 0; i < TheOptimals.size(); i++)
//	{
//		messageToSlaveToGetRealImage mtsR{ TheOptimals.at(i) };
//		ResponseToGetRealImageFromSlave RealI = mySlaves[TheOptimals.at(i).GetSlaveId()].return_of_an_image_by_its_properties(mtsR);
//		CHECK(manager.checking_if_there_is_a_real_picture(RealI) == true);
//		manager.getTheRealImageFromSlave(RealI, realImages);
//	}
//	RequestToReceiveAMapFromOpenCV ROpenCV{ realImages };
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////
//	vector<RealImage> realImages1;
//	vector<ImageFromSlave> TheOptimals1;
//	mapRequest MapReq1;
//	SlavesManager manager1;
//	float MinimumLine1;
//	getRequestForMap getreqMap1 = { MapReq1 };
//	PointForSlave pfs1 = manager1.getARequestForAMap(getreqMap1, MinimumLine1);
//
//	CHECK(manager1.The_request_is_processed(MapReq1.getReqId()) == true);
//
//#define INVALIDNUM -1
//
//	while (pfs1.GetX() != INVALIDNUM && pfs1.GetY() != INVALIDNUM)//כל עוד זה לא סוף התמונה
//	{
//		vector<ImageFromSlave> TheImagesFromSlaves = { {0,0,0,0,0,0} };
//		vector<ResponseToGetImageFromSlave> resultFromSlaves = {};
//
//#define ARRAYSIZE 5
//		vector<bool> theBoolAnswers(ARRAYSIZE, false);
//
//		int NumOfSlaves_HasToAnswer = ARRAYSIZE;
//
//		Slave mySlaves[ARRAYSIZE];
//		for (int i = 0; i < ARRAYSIZE; i++)
//		{
//			if (mySlaves[i].GetId() > 5)
//				mySlaves[i].setId(i);
//
//			messageToSlaveToGetImage msg = { pfs1,i, MapReq1.getReqId() };
//			ResponseToGetImageFromSlave resFromSlaveToTheMaster = mySlaves[i].GetAStructForImageInPoint(msg);
//			if (resFromSlaveToTheMaster.HaveOrNot_Image)
//				CHECK(manager1.The_request_is_processed(resFromSlaveToTheMaster.imgRes.at(0).GetReqId()) == true);
//			ResponseToGetImageFromSlave res =
//				manager1.getResponse_ImageFromSlave(resFromSlaveToTheMaster, TheImagesFromSlaves,
//					NumOfSlaves_HasToAnswer, theBoolAnswers);
//		}
//
//		//check that at least one answered yet
//		CHECK(manager1.handle_photo_request(theBoolAnswers) == false);
//
//		//check that everyone answered
//		CHECK(manager1.no_comments(NumOfSlaves_HasToAnswer) == true);
//
//		ImageFromSlave TheChosenImage = manager1.ChooseTheOptimalImage(TheImagesFromSlaves, TheOptimals1);
//		pfs1 = manager1.next_point_to_send(TheChosenImage,
//			MapReq1, MinimumLine1, TheChosenImage.GetBR().GetX());
//	}
//
//	CHECK(TheOptimals1[0].GetBR().GetY() == Temporary8[0].GetBR().GetY());
//	CHECK(TheOptimals1[0].GetBR().GetX() == Temporary8[0].GetBR().GetX());
//	CHECK(TheOptimals1[0].GetTL().GetY() == Temporary8[0].GetTL().GetY());
//	CHECK(TheOptimals1[0].GetTL().GetX() == Temporary8[0].GetTL().GetX());
//										 
//
//	Slave mySlaves1[10] = {};
//	for (int i = 0; i < 10; i++)
//	{
//		mySlaves1[i].setId(i);
//	}
//	for (size_t i = 0; i < TheOptimals1.size(); i++)
//	{
//		messageToSlaveToGetRealImage mtsR{ TheOptimals1.at(i) };
//		ResponseToGetRealImageFromSlave RealI = mySlaves1[TheOptimals1.at(i).GetSlaveId()].return_of_an_image_by_its_properties(mtsR);
//		CHECK(manager1.checking_if_there_is_a_real_picture(RealI) == true);
//		manager1.getTheRealImageFromSlave(RealI, realImages1);
//	}
//	RequestToReceiveAMapFromOpenCV ROpenCV1{ realImages1 };
//	//קבלת המפה
//}
//
////טסטים על פונקציות בנפרד
//
////בדיקה האם הרחפן מטפל כראוי וידע להחזיר תמונה כאשר היא קיימת.
//TEST_CASE("slave handle the request well")
//{
//	ImageFromSlave temporaryforme[5] =
//	{
//		{ 32.7897f, 35.5785f, 32.7915f, 35.5809f,1,8 },
//		{ 32.7880f, 35.5733f, 32.7902f, 35.5757f,1,8 },
//		{ 32.7925f, 35.5769f, 32.7948f, 35.5792f,3,8 },
//		{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,3,8 },
//		{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,5,8 }
//	};
//
//	ImageFromSlave ansimg[1] =
//	{
//		{ 32.7897f, 35.5785f, 32.7915f, 35.5809f,1,8 },
//	};
//
//
//	Slave s;
//	s.setId(1);
//	
//	PointForSlave p(32.7897f, 35.5785f);
//	vector<ImageFromSlave> img;
//	messageToSlaveToGetImage m{ p,1,56 };
//	ResponseToGetImageFromSlave res =  s.GetAStructForImageInPoint(m);
//
//	/*ImageFromSlave forCounter;
//	int fC = forCounter.getCounter();*/
//	
//	
//
//
//	CHECK(res.imgRes[0].GetBR().GetY() == ansimg[0].GetBR().GetY());
//	CHECK(res.imgRes[0].GetBR().GetX() == ansimg[0].GetBR().GetX());
//	CHECK(res.imgRes[0].GetTL().GetY() == ansimg[0].GetTL().GetY());
//	CHECK(res.imgRes[0].GetTL().GetX() == ansimg[0].GetTL().GetX());
//
//}
////
////
////
//// שנבחרה התמונה האופטימלית בדיקה
//TEST_CASE("check that the optimal image chosen")
//{
//	vector<ImageFromSlave> TheOptimal;
//	vector<ImageFromSlave> TheImgProp =
//	{
//		{ 32.7897f, 35.5785f, 32.7915f, 35.5809f,4,8 },
//		{ 32.7880f, 35.5733f, 32.7902f, 35.5757f,9,8 },
//		{ 32.7925f, 35.5769f, 32.7948f, 35.5792f,3,8 },
//		{ 32.7941f, 35.5812f, 32.7963f, 35.5841f,7,8 },
//		{ 32.7893f, 35.5749f, 32.7916f, 35.5780f,4,8 }
//	};
//
//	SlavesManager manager;
//	ImageFromSlave img = manager.ChooseTheOptimalImage(TheImgProp,TheOptimal);
//	
//
//	CHECK(img.GetSlaveId() == 7);
//
//}
//

